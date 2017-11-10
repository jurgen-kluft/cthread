#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"
#include "xbase\x_string_std.h"

#include "xmthread\x_threadpool.h"
#include "xmthread\x_runnable.h"
#include "xmthread\x_thread.h"
#include "xmthread\x_event.h"
#include "xmthread\x_threadlocal.h"

#include "xtime\x_datetime.h"
#include "xtime\x_timespan.h"

namespace xcore
{
	static void copy_str(char* to, s32 max_len, const char* from)
	{
		for (s32 i=0; i<max_len; ++i)
		{
			to[i] = from[i];
			if (from[i] == '\0')
				break;
		}
	}

	struct xthreadvec
	{
		static xthreadvec*	sCreate(x_iallocator* allocator, u32 maxthreads)
		{
			xthreadvec* tv = (xthreadvec*)allocator->allocate(sizeof(xthreadvec) + maxthreads*sizeof(xpoolthread*), 4);
			tv->_numthreads = 0;
			tv->_maxthreads = maxthreads;
			tv->_threads    = (xpoolthread**)(tv+1);
			return tv;
		}

		static void			sDestroy(x_iallocator* allocator, xthreadvec* tv)
		{
			allocator->deallocate(tv);
		}

		inline void			clear()
		{
			_numthreads = 0;
			for (u32 i=0; i<_maxthreads; ++i)
				_threads[i]=NULL;
		}

		inline u32			size() const
		{
			return _numthreads; 
		}

		inline void			push_back(xpoolthread* pThread)
		{
			_threads[_numthreads++] = pThread;
		}

		inline xpoolthread* pop_back()
		{
			if (_numthreads == 0)
				return NULL;
			--_numthreads;
			xpoolthread* pt = _threads[_numthreads];
			_threads[_numthreads] = NULL;
			return pt;
		}

		inline xpoolthread* remove(u32 pos)
		{
			xpoolthread* pt = NULL;
			if (pos>=0)
			{
				pt = _threads[pos];
				for (u32 j=pos+1; j<_numthreads; ++j)
					_threads[j-1] = _threads[j];

				--_numthreads;
				_threads[_numthreads] = NULL;
			}
			return pt;
		}

		inline xpoolthread* operator [] (u32 index) { ASSERT(index<_numthreads); return _threads[index]; }

		u32				_numthreads;
		u32				_maxthreads;
		xpoolthread**	_threads;
	};



	class xpoolthread : public xrunnable
	{
	public:
						xpoolthread(const char* name, s32 stackSize = XMTHREAD_STACK_SIZE);
						~xpoolthread();

		void			start();
		void			start(xthread::Priority priority, xrunnable& target);
		void			start(xthread::Priority priority, xrunnable& target, const char* name);
		bool			idle() const;
		u64				idleTime(xtimespan const& now);
		void			join();
		void			activate();
		void			release();
		void			run();

	private:
		volatile bool	_idle;
		xtimespan		_idleTime;
		xrunnable*		_pTarget;
		char			_name[64];
		xthread			_thread;
		xevent			_targetReady;
		xevent			_targetCompleted;
		xevent			_started;
		xfastmutex		_mutex;
	};


	xpoolthread::xpoolthread(const char* name, s32 stackSize)
		: _idle(true)
		, _idleTime(0)
		, _pTarget(0)
		, _thread(name)
		, _targetCompleted(false)
	{
		copy_str(_name, sizeof(_name), name);
		ASSERT (stackSize >= 0);
		_thread.setStackSize(stackSize);
	}

	xpoolthread::~xpoolthread()
	{
	}

	void xpoolthread::start()
	{
		_thread.start(*this);
		_started.wait();
	}

	void xpoolthread::start(xthread::Priority priority, xrunnable& target)
	{
		xfastmutex::xscoped_lock lock(_mutex);

		ASSERT (_pTarget == 0);

		_pTarget = &target;
		_thread.setPriority(priority);
		_targetReady.set();
	}

	void xpoolthread::start(xthread::Priority priority, xrunnable& target, const char* name)
	{
		xfastmutex::xscoped_lock lock(_mutex);

		char fullName[128];
		if (name==NULL || name[0]=='\0')
		{
			x_sprintf(fullName, sizeof(fullName)-1, "%s (%s)", _name, _name);
		}
		else
		{
			x_sprintf(fullName, sizeof(fullName)-1, "%s (%s)", name, _name);
		}
		_thread.setName(fullName);
		_thread.setPriority(priority);

		ASSERT (_pTarget == 0);

		_pTarget = &target;
		_targetReady.set();
	}

	inline bool xpoolthread::idle() const
	{
		return _idle;
	}

	u64 xpoolthread::idleTime(xtimespan const& now)
	{
		xfastmutex::xscoped_lock lock(_mutex);
		xtimespan idleTime = now - _idleTime;
		return idleTime.totalSeconds();
	}

	void xpoolthread::join()
	{
		_mutex.lock();
		xrunnable* pTarget = _pTarget;
		_mutex.unlock();
		if (pTarget)
			_targetCompleted.wait();
	}

	void xpoolthread::activate()
	{
		xfastmutex::xscoped_lock lock(_mutex);

		ASSERT (_idle);
		_idle = false;
		_targetCompleted.reset();
	}

	void xpoolthread::release()
	{
		const u32 JOIN_TIMEOUT = 10000;

		_mutex.lock();
		_pTarget = 0;
		_mutex.unlock();

		_targetReady.set();
		if (_thread.tryJoin(JOIN_TIMEOUT))
		{
			delete this;
		}
	}

	void xpoolthread::run()
	{
		_started.set();
		for (;;)
		{
			_targetReady.wait();
			_mutex.lock();
			if (_pTarget) // a NULL target means kill yourself
			{
				_mutex.unlock();
				{
					_pTarget->run();
				}
				xfastmutex::xscoped_lock lock(_mutex);
				_pTarget  = 0;
				_idleTime = xtimespan(xdatetime::sNow().ticks());
				_idle     = true;
				_targetCompleted.set();
				_thread.setName(_name);
				_thread.setPriority(xthread::PRIO_NORMAL);
			}
			else
			{
				_mutex.unlock();
				break;
			}
		}
	}


	xthreadpool::xthreadpool(x_iallocator* allocator, u32 minCapacity, u32 maxCapacity, u32 idleTime, u32 stackSize)
		: _allocator(allocator)
		, _minCapacity(minCapacity)
		, _maxCapacity(maxCapacity)
		, _idleTime(idleTime)
		, _serial(0)
		, _age(0)
		, _stackSize(stackSize)
		, _threads(NULL)
	{
		ASSERT (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);

		_name[0] = '\0';
		_name[1] = '\0';

		_threads = xthreadvec::sCreate(allocator, maxCapacity);
		for (u32 i = 0; i < _minCapacity; i++)
		{
			xpoolthread* pThread = createThread();
			_threads->push_back(pThread);
			pThread->start();
		}
	}

	xthreadpool::xthreadpool(x_iallocator* allocator, const char* name, u32 minCapacity, u32 maxCapacity, u32 idleTime, u32 stackSize)
		: _allocator(allocator)
		, _minCapacity(minCapacity)
		, _maxCapacity(maxCapacity)
		, _idleTime(idleTime)
		, _serial(0)
		, _age(0)
		, _stackSize(stackSize)
		, _threads(NULL)
	{
		ASSERT (minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0);
		copy_str(_name, sizeof(_name), name);

		_threads = xthreadvec::sCreate(allocator, maxCapacity);
		for (u32 i = 0; i < _minCapacity; i++)
		{
			xpoolthread* pThread = createThread();
			_threads->push_back(pThread);
			pThread->start();
		}
	}

	xthreadpool::~xthreadpool()
	{
		stopAll();
		xthreadvec::sDestroy(_allocator, _threads);
	}

	u32 xthreadpool::capacity() const
	{
		xfastmutex::xscoped_lock lock(_mutex);
		return _maxCapacity;
	}

	u32 xthreadpool::available() const
	{
		xfastmutex::xscoped_lock lock(_mutex);

		s32 count = 0;
		for (u32 i=0; i<_threads->size(); ++i)
		{
			xpoolthread* pt = (*_threads)[i];
			if (pt->idle()) 
				++count;
		}
		return (s32) (count + _maxCapacity - _threads->size());
	}

	u32 xthreadpool::used() const
	{
		xfastmutex::xscoped_lock lock(_mutex);

		s32 count = 0;
		for (u32 i=0; i<_threads->size(); ++i)
		{
			xpoolthread* pt = (*_threads)[i];
			if (!pt->idle()) 
				++count;
		}
		return count;
	}

	u32 xthreadpool::allocated() const
	{
		xfastmutex::xscoped_lock lock(_mutex);

		return s32(_threads->size());
	}


	bool xthreadpool::start(xrunnable& target)
	{
		xpoolthread* pt = getThread();
		bool started = (pt!=NULL);
		if (started)
			pt->start(xthread::PRIO_NORMAL, target);
		return started;
	}

	bool xthreadpool::start(xrunnable& target, const char* name)
	{
		xpoolthread* pt = getThread();
		bool started = (pt!=NULL);
		if (started)
			pt->start(xthread::PRIO_NORMAL, target, name);
		return started;
	}

	bool xthreadpool::startWithPriority(xthread::Priority priority, xrunnable& target)
	{
		xpoolthread* pt = getThread();
		bool started = (pt!=NULL);
		if (started)
			pt->start(priority, target);
		return started;
	}

	bool xthreadpool::startWithPriority(xthread::Priority priority, xrunnable& target, const char* name)
	{
		xpoolthread* pt = getThread();
		bool started = (pt!=NULL);
		if (started)
			pt->start(priority, target, name);
		return started;
	}

	void xthreadpool::stopAll()
	{
		xfastmutex::xscoped_lock lock(_mutex);

		for (u32 i=0; i<_threads->size(); ++i)
		{
			xpoolthread* pt = (*_threads)[i];
			pt->release();
		}
		_threads->clear();
	}

	void xthreadpool::joinAll()
	{
		xfastmutex::xscoped_lock lock(_mutex);

		for (u32 i=0; i<_threads->size(); ++i)
		{
			xpoolthread* pt = (*_threads)[i];
			pt->join();
		}
		housekeep();
	}

	void xthreadpool::collect()
	{
		xfastmutex::xscoped_lock lock(_mutex);
		housekeep();
	}

	void xthreadpool::housekeep()
	{
		_age = 0;
		if (_threads->size() <= _minCapacity)
			return;

		// Remember the 'now' time
		xtimespan now = xtimespan(xdatetime::sNow().ticks());

		// In-place sort of the thread array into 3 categories:
		//     1. Active threads
		//     2. Idle threads
		//     3. Expired threads

		u32 n = _threads->size();
		
		// We sort the array in 2 steps:
		//     1. We remove and add (at the back) the idle threads
		//     2. We remove and add (at the back) the expired threads

		// Step 1. Put the idle threads at the end of the array
		u32 num_idle = 0;
		u32 num_active = 0;
		for (u32 i=0; i<n; )
		{
			xpoolthread* pt = (*_threads)[i];
			if (pt->idle())
			{
				u64 const idleTime = pt->idleTime(now);
				if (idleTime < _idleTime)
				{
					// Idle threads
					--n;
					++num_idle;
					pt = _threads->remove(i);
					_threads->push_back(pt);
				}
				else
				{
					++i;
				}
			}
			else
			{
				++i;
				++num_active;
			}
		}

		// Step 2. Put the expired threads at the end of the array
		u32 num_expired = 0;
		for (u32 i=0; i<n; )
		{
			xpoolthread* pt = (*_threads)[i];
			if (pt->idle())
			{
				u64 const idleTime = pt->idleTime(now);
				if (idleTime >= _idleTime)
				{
					// Expired threads
					--n;
					++num_expired;
					pt = _threads->remove(i);
					_threads->push_back(pt);
				}
				else
				{
					++i;
				}
			}
			else
			{
				++i;
			}
		}

		// The sum of active, idle and expired threads has to add-up to
		// the total number of threads.
		ASSERT((num_idle + num_active + num_expired) == _threads->size());

		// Now the array is sorted as follows:
		// - active
		// - idle
		// - expired
		u32 limit = num_idle + num_active;
		if (limit < _minCapacity)
			limit = _minCapacity;

		// Kill some threads
		n = _threads->size();
		if (n > limit)
		{
			s32 n_to_kill = n - limit;
			while (n_to_kill-- > 0)
			{
				xpoolthread* pt = _threads->pop_back();
				pt->release();
			}
		}
	}

	xpoolthread* xthreadpool::getThread()
	{
		xfastmutex::xscoped_lock lock(_mutex);

		if (++_age == 32)
			housekeep();

		xpoolthread* pThread = 0;
		for (u32 i=0; i<_threads->size(); ++i)
		{
			xpoolthread* pt = (*_threads)[i];
			if (pt->idle()) 
			{
				pThread = pt;
				break;
			}
		}

		if (!pThread)
		{
			if (_threads->size() < _maxCapacity)
			{
				pThread = createThread();
				{
					pThread->start();
					_threads->push_back(pThread);
				}
			}
			else
			{
				// no thread available
				return NULL;
			}
		}
		pThread->activate();
		return pThread;
	}

	xpoolthread* xthreadpool::createThread()
	{
		char name[128];
		x_sprintf(name, sizeof(name)-1, "%s[#%d]", _name, ++_serial);
		return new xpoolthread(name, _stackSize);
	}

} // namespace xcore
