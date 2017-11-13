#ifndef __XMTHREAD_NAMEDMUTEX_H__
#define __XMTHREAD_NAMEDMUTEX_H__

#include "xbase\x_target.h"
#include "xmthread\x_ScopedLock.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_namedmutex_win32.h"
#endif

namespace xcore 
{
	/// A xnmutex (mutual exclusion) is a global synchronization 
	/// mechanism used to control access to a shared resource
	/// in a concurrent (multi process) scenario.
	/// Using the xscoped_lock class is the preferred way to automatically
	/// lock and unlock a mutex.
	///
	/// Unlike a xmutex or a xfastmutex, which itself is the unit of synchronization,
	/// a xnmutex refers to a named operating system resource being the
	/// unit of synchronization.
	/// In other words, there can be multiple instances of xnmutex referring
	/// to the same actual synchronization object.
	///
	///
	/// There should not be more than one instance of xnmutex for
	/// a given name in a process. Otherwise, the instances may
	/// interfere with each other.
	class xnmutex : private xnmutex_impl
	{
	public:
		typedef xcore::xscoped_lock<xnmutex> xscoped_lock;

		xnmutex(const char* name);
		/// creates the xmutex.

		~xnmutex();
		/// destroys the xmutex.

		void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another process or thread.

		bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another process or thread.
		/// Returns true if the mutex was successfully locked.

		void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

	private:
		xnmutex();
		xnmutex(const xnmutex&);
		xnmutex& operator = (const xnmutex&);
	};


	//
	// inlines
	//
	inline void xnmutex::lock()
	{
		lockImpl();
	}


	inline bool xnmutex::tryLock()
	{
		return tryLockImpl();
	}


	inline void xnmutex::unlock()
	{
		unlockImpl();
	}


} // namespace xcore


#endif // __XMTHREAD_NAMEDMUTEX_H__
