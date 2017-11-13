#ifndef __XMTHREAD_CONDITION_H__
#define __XMTHREAD_CONDITION_H__

#include "xbase\x_target.h"
#include "xmthread\x_Mutex.h"
#include "xmthread\x_ScopedUnlock.h"
#include "xmthread\x_Event.h"

namespace xcore 
{
	/// A xcondition is a synchronization object used to block a thread 
	/// until a particular condition is met. 
	/// A xcondition object is always used in conjunction with
	/// a xmutex (or xfastmutex) object.
	///
	/// xcondition objects are similar to POSIX condition variables, which the
	/// difference that xcondition is not subject to spurious wakeups.
	///
	/// Threads waiting on a xcondition are resumed in FIFO order.
	class xcondition
	{
	public:
		xcondition();
		/// Creates the xcondition.

		~xcondition();
		/// Destroys the xcondition.

		/// Unlocks the mutex (which must be locked upon calling
		/// wait()) and waits until the xcondition is signalled.
		///
		/// The given mutex will be locked again upon 
		/// leaving the function, even in case of an exception.
		template <class Mtx>
		void		wait(Mtx& mutex);

		/// Unlocks the mutex (which must be locked upon calling
		/// wait()) and waits for the given time until the xcondition is signalled.
		///
		/// The given mutex will be locked again upon successfully leaving the 
		/// function, even in case of an exception.
		///
		/// Throws a TimeoutException if the xcondition is not signalled
		/// within the given time interval.
		template <class Mtx>
		void		wait(Mtx& mutex, u32 milliseconds);

		/// Unlocks the mutex (which must be locked upon calling
		/// tryWait()) and waits for the given time until the xcondition is signalled.
		///
		/// The given mutex will be locked again upon leaving the 
		/// function, even in case of an exception.
		///
		/// Returns true if the xcondition has been signalled
		/// within the given time interval, otherwise false.
		template <class Mtx>
		bool		tryWait(Mtx& mutex, u32 milliseconds);

		void		signal();
		/// Signals the xcondition and allows one waiting thread
		/// to continue execution.

		void		broadcast();
		/// Signals the xcondition and allows all waiting
		/// threads to continue their execution.

	protected:
		struct waiter
		{
			inline		waiter() : _prev(0), _next(0) {}

			xevent		_event;
			waiter*		_prev;
			waiter*		_next;
		};

		void		enqueue(waiter*);
		waiter*		dequeue();
		void		dequeue(waiter&);

	private:
					xcondition(const xcondition&);
					xcondition& operator = (const xcondition&);

		xfastmutex	_mutex;
		waiter*		_waiters;
	};


	template <class Mtx>
	void		xcondition::wait(Mtx& mutex)
	{
		xscopedunlock<Mtx> unlock(mutex, false);
		waiter wtr;
		{
			xfastmutex::xscoped_lock lock(_mutex);
			mutex.unlock();
			enqueue(&wtr);
		}
		wtr._event.wait();
	}

	/// Unlocks the mutex (which must be locked upon calling
	/// wait()) and waits for the given time until the xcondition is signalled.
	///
	/// The given mutex will be locked again upon successfully leaving the 
	/// function, even in case of an exception.
	///
	/// Throws a TimeoutException if the xcondition is not signalled
	/// within the given time interval.
	template <class Mtx>
	void		xcondition::wait(Mtx& mutex, u32 milliseconds)
	{
		if (!tryWait(mutex, milliseconds))
		{
			// timeout
		}
	}

	/// Unlocks the mutex (which must be locked upon calling
	/// tryWait()) and waits for the given time until the xcondition is signalled.
	///
	/// The given mutex will be locked again upon leaving the 
	/// function, even in case of an exception.
	///
	/// Returns true if the xcondition has been signalled
	/// within the given time interval, otherwise false.
	template <class Mtx>
	bool		xcondition::tryWait(Mtx& mutex, u32 milliseconds)
	{
		xscopedunlock<Mtx> unlock(mutex, false);
		waiter wtr;
		{
			xfastmutex::xscoped_lock lock(_mutex);
			mutex.unlock();
			enqueue(&wtr);
			ASSERT(_waiters!=NULL);
		}

		bool result = wtr._event.tryWait(milliseconds);
		/// Hmmmm, if we failed here we where removed by the signal/broadcast?
		if (!result)
		{
			xfastmutex::xscoped_lock lock(_mutex);
			ASSERT(_waiters!=NULL);
			dequeue(wtr);
		}
		return result;
	}

} // namespace xcore


#endif // __XMTHREAD_CONDITION_H__
