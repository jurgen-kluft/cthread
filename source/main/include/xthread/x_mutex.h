#ifndef __XMTHREAD_MUTEX_H__
#define __XMTHREAD_MUTEX_H__

#include "xbase/x_target.h"

#if defined(TARGET_PC)
#include "xthread/private/windows/x_mutex_win.h"
#elif defined(TARGET_OSX)
#include "xthread/private/osx/x_mutex_osx.h"
#endif


namespace xcore 
{
	// A xmutex (mutual exclusion) is a synchronization 
	// mechanism used to control access to a shared resource
	// in a concurrent (multithreaded) scenario.
	// Mutexes are recursive, that is, the same mutex can be 
	// locked multiple times by the same thread (but, of course,
	// not by other threads).
	// Using the xscoped_lock class is the preferred way to automatically
	// lock and unlock a mutex.
	class xmutex : private xmutex_impl
	{
	public:
				xmutex() : xmutex_impl() {}		// creates the xmutex.
				~xmutex() {}					// destroys the xmutex.

		void	lock();
		// Locks the mutex. Blocks if the mutex
		// is held by another thread.

		bool	tryLock();
		// Tries to lock the mutex. Returns false immediately
		// if the mutex is already held by another thread.
		// Returns true if the mutex was successfully locked.

		void	unlock();
		// Unlocks the mutex so that it can be acquired by
		// other threads.

	private:
				xmutex(const xmutex&) {}
		xmutex& operator = (const xmutex&) {}
	};


	//
	// inlines
	//
	inline void xmutex::lock()
	{
		mutex_lock();
	}

	inline bool xmutex::tryLock()
	{
		return mutex_tryLock();
	}

	inline void xmutex::unlock()
	{
		mutex_unlock();
	}


} // namespace xcore


#endif // __XMTHREAD_MUTEX_H__
