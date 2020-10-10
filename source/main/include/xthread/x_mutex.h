#ifndef __XMTHREAD_MUTEX_H__
#define __XMTHREAD_MUTEX_H__

#include "xbase/x_target.h"

namespace xcore 
{
	#if defined(TARGET_PC)
		const int xmutex_data_size64 = 14;
	#elif defined(TARGET_MAC)
		const int xmutex_data_size64 = 10;
	#else
		const int xmutex_data_size64 = -1;
	#endif

	// A xmutex (mutual exclusion) is a synchronization 
	// mechanism used to control access to a shared resource
	// in a concurrent (multithreaded) scenario.
	// Mutexes are recursive, that is, the same mutex can be 
	// locked multiple times by the same thread (but, of course,
	// not by other threads).
	// Using the xscoped_lock class is the preferred way to automatically
	// lock and unlock a mutex.
	class xmutex
	{
	public:
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
		u64		m_data[xmutex_data_size64];

		xmutex() {}
        ~xmutex() {}
        xmutex(const xmutex&) {}
		xmutex& operator = (const xmutex&) {}
	};


} // namespace xcore


#endif // __XMTHREAD_MUTEX_H__
