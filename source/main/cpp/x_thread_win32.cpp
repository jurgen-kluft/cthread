#include "xmthread\x_thread.h"
#include <process.h>

namespace xcore 
{
	xthread_impl::CurrentThreadHolder xthread_impl::_currentThreadHolder;

	xthread_impl::xthread_impl()
		: _pRunnableTarget(0)
		, _thread(0)
		, _threadId(0)
		, _prio(PRIO_NORMAL_IMPL)
		, _stackSize(XMTHREAD_STACK_SIZE)
	{
	}


	xthread_impl::~xthread_impl()
	{
		if (_thread)
			CloseHandle(_thread);
	}


	void xthread_impl::setPriorityImpl(s32 prio)
	{
		if (prio != _prio)
		{
			_prio = prio;
			if (_thread)
			{
				if (SetThreadPriority(_thread, _prio) == 0)
				{
					// cannot set thread priority
				}
			}
		}
	}


	void xthread_impl::setOSPriorityImpl(s32 prio)
	{
		setPriorityImpl(prio);
	}


	void xthread_impl::startImpl(xrunnable& target)
	{
		if (isRunningImpl())
		{
			// thread already running
		}

		_pRunnableTarget = &target;

		createImpl(runnableEntry, this);
	}


	void xthread_impl::startImpl(Callable target, void* pData)
	{
		if (isRunningImpl())
		{
			// thread already running
		}

		threadCleanup();
		_callbackTarget.callback = target;
		_callbackTarget.pData = pData;

		createImpl(callableEntry, this);
	}


	void xthread_impl::createImpl(Entry ent, void* pData)
	{
		u32 threadId;
		_thread = (HANDLE) _beginthreadex(NULL, _stackSize, ent, this, 0, &threadId);
		_threadId = static_cast<DWORD>(threadId);

		if (!_thread)
		{
			// cannot create thread
		}
		if (_prio != PRIO_NORMAL_IMPL && !SetThreadPriority(_thread, _prio))
		{
			// cannot set thread priority
		}
	}


	void xthread_impl::joinImpl()
	{
		if (!_thread)
			return;

		switch (WaitForSingleObject(_thread, INFINITE))
		{
		case WAIT_OBJECT_0:
			threadCleanup();
			return;
		default:
			// cannot join thread
			break;
		}
	}


	bool xthread_impl::joinImpl(u32 milliseconds)
	{
		if (!_thread)
			return true;

		switch (WaitForSingleObject(_thread, milliseconds + 1))
		{
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			threadCleanup();
			return true;
		default:
			// cannot join thread
			return false;
		}
	}


	bool xthread_impl::isRunningImpl() const
	{
		if (_thread)
		{
			DWORD ec = 0;
			return GetExitCodeThread(_thread, &ec) && ec == STILL_ACTIVE;
		}
		return false;
	}


	void xthread_impl::threadCleanup()
	{
		if (!_thread)
			return;
		if (CloseHandle(_thread))
			_thread = 0;
	}


	xthread_impl* xthread_impl::currentImpl()
	{
		return _currentThreadHolder.get();
	}


	xthread_impl::TIDImpl xthread_impl::currentTidImpl()
	{
		return GetCurrentThreadId();
	}


	u32 __stdcall xthread_impl::runnableEntry(void* pThread)
	{
		_currentThreadHolder.set(reinterpret_cast<xthread_impl*>(pThread));
		{
			reinterpret_cast<xthread_impl*>(pThread)->_pRunnableTarget->run();
		}
		return 0;
	}


	u32 __stdcall xthread_impl::callableEntry(void* pThread)
	{
		_currentThreadHolder.set(reinterpret_cast<xthread_impl*>(pThread));
		{
			xthread_impl* pTI = reinterpret_cast<xthread_impl*>(pThread);
			pTI->_callbackTarget.callback(pTI->_callbackTarget.pData);
		}
		return 0;
	}


} // namespace xcore
