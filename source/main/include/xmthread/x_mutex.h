#ifndef __XMTHREAD_MUTEX_H__
#define __XMTHREAD_MUTEX_H__

#include "xbase\x_target.h"
#include "xmthread\x_ScopedLock.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_mutex_win32.h"
#endif


namespace xcore 
{
	/// A xmutex (mutual exclusion) is a synchronization 
	/// mechanism used to control access to a shared resource
	/// in a concurrent (multithreaded) scenario.
	/// Mutexes are recursive, that is, the same mutex can be 
	/// locked multiple times by the same thread (but, of course,
	/// not by other threads).
	/// Using the xscoped_lock class is the preferred way to automatically
	/// lock and unlock a mutex.
	class xmutex : private xmutex_impl
	{
	public:
		typedef xcore::xscoped_lock<xmutex> xscoped_lock;

		xmutex();
		/// creates the xmutex.

		~xmutex();
		/// destroys the xmutex.

		void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

		void lock(u32 milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread. Throws a TimeoutException
		/// if the mutex can not be locked within the given timeout.
		///
		/// Performance Note: On most platforms (including Windows), this member function is 
		/// implemented using a loop calling (the equivalent of) tryLock() and xthread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

		bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another thread.
		/// Returns true if the mutex was successfully locked.

		bool tryLock(u32 milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread.
		/// Returns true if the mutex was successfully locked.
		///
		/// Performance Note: On most platforms (including Windows), this member function is 
		/// implemented using a loop calling (the equivalent of) tryLock() and xthread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

		void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

	private:
		xmutex(const xmutex&);
		xmutex& operator = (const xmutex&);
	};


	class xfastmutex: private FastMutexImpl
		/// A xfastmutex (mutual exclusion) is similar to a xmutex.
		/// Unlike a xmutex, however, a xfastmutex is not recursive,
		/// which means that a deadlock will occur if the same
		/// thread tries to lock a mutex it has already locked again.
		/// Locking a xfastmutex is faster than locking a recursive xmutex.
		/// Using the xscoped_lock class is the preferred way to automatically
		/// lock and unlock a mutex.
	{
	public:
		typedef xcore::xscoped_lock<xfastmutex> xscoped_lock;

		xfastmutex();
		/// creates the xmutex.

		~xfastmutex();
		/// destroys the xmutex.

		void lock();
		/// Locks the mutex. Blocks if the mutex
		/// is held by another thread.

		void lock(u32 milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread. Throws a TimeoutException
		/// if the mutex can not be locked within the given timeout.
		///
		/// Performance Note: On most platforms (including Windows), this member function is 
		/// implemented using a loop calling (the equivalent of) tryLock() and xthread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

		bool tryLock();
		/// Tries to lock the mutex. Returns false immediately
		/// if the mutex is already held by another thread.
		/// Returns true if the mutex was successfully locked.

		bool tryLock(u32 milliseconds);
		/// Locks the mutex. Blocks up to the given number of milliseconds
		/// if the mutex is held by another thread.
		/// Returns true if the mutex was successfully locked.
		///
		/// Performance Note: On most platforms (including Windows), this member function is 
		/// implemented using a loop calling (the equivalent of) tryLock() and xthread::sleep().
		/// On POSIX platforms that support pthread_mutex_timedlock(), this is used.

		void unlock();
		/// Unlocks the mutex so that it can be acquired by
		/// other threads.

	private:
		xfastmutex(const xfastmutex&);
		xfastmutex& operator = (const xfastmutex&);
	};


	class NullMutex
		/// A NullMutex is an simple mutex implementation
		/// which performs no locking at all. Useful in policy driven design
		/// where the type of mutex used can be now a template parameter allowing 
		/// to switch between thread-safe and not thread-safe implementations.
	{
	public:
		typedef xcore::xscoped_lock<NullMutex> xscoped_lock;

		NullMutex()
			/// Creates the NullMutex.
		{
		}

		~NullMutex()
			/// Destroys the NullMutex.
		{
		}

		void lock()
			/// Does nothing.
		{
		}

		void lock(u32)
			/// Does nothing.
		{
		}

		bool tryLock()
			/// Does nothing and always returns true.
		{
			return true;
		}

		bool tryLock(u32)
			/// Does nothing and always returns true.
		{
			return true;
		}

		void unlock()
			/// Does nothing.
		{
		}
	};


	//
	// inlines
	//
	inline void xmutex::lock()
	{
		lockImpl();
	}


	inline void xmutex::lock(u32 milliseconds)
	{
		tryLockImpl(milliseconds);
	}


	inline bool xmutex::tryLock()
	{
		return tryLockImpl();
	}


	inline bool xmutex::tryLock(u32 milliseconds)
	{
		return tryLockImpl(milliseconds);
	}


	inline void xmutex::unlock()
	{
		unlockImpl();
	}


	inline void xfastmutex::lock()
	{
		lockImpl();
	}


	inline void xfastmutex::lock(u32 milliseconds)
	{
		tryLockImpl(milliseconds);
	}


	inline bool xfastmutex::tryLock()
	{
		return tryLockImpl();
	}


	inline bool xfastmutex::tryLock(u32 milliseconds)
	{
		return tryLockImpl(milliseconds);
	}


	inline void xfastmutex::unlock()
	{
		unlockImpl();
	}


} // namespace xcore


#endif // __XMTHREAD_MUTEX_H__
