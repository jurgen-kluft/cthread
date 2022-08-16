#ifndef __XMTHREAD_MUTEX_H__
#define __XMTHREAD_MUTEX_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore 
{
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

	protected:
		xmutex();
		~xmutex();
        xmutex(const xmutex&) {}
		xmutex& operator = (const xmutex&) { return *this; }
	};


} // namespace ncore


#endif // __XMTHREAD_MUTEX_H__
