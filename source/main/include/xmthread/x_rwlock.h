#ifndef __XMTHREAD_RWLOCK_H__
#define __XMTHREAD_RWLOCK_H__
#include "xbase\x_target.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_rwlock_win32.h"
#endif

namespace xcore 
{
	class ScopedRWLock;
	class ScopedReadRWLock;
	class ScopedWriteRWLock;

	// A reader writer lock allows multiple concurrent
	// readers or one exclusive writer.
	class xrwlock : private xrwlock_impl
	{
	public:
		typedef ScopedRWLock xscoped_lock;
		typedef ScopedReadRWLock ScopedReadLock;
		typedef ScopedWriteRWLock ScopedWriteLock;

		xrwlock();
		/// Creates the Reader/Writer lock.

		~xrwlock();
		/// Destroys the Reader/Writer lock.

		void readLock();
		/// Acquires a read lock. If another thread currently holds a write lock,
		/// waits until the write lock is released.

		bool tryReadLock();
		/// Tries to acquire a read lock. Immediately returns true if successful, or
		/// false if another thread currently holds a write lock.

		void writeLock();
		/// Acquires a write lock. If one or more other threads currently hold 
		/// locks, waits until all locks are released. The results are undefined
		/// if the same thread already holds a read or write lock

		bool tryWriteLock();
		/// Tries to acquire a write lock. Immediately returns true if successful,
		/// or false if one or more other threads currently hold 
		/// locks. The result is undefined if the same thread already
		/// holds a read or write lock.

		void unlock();
		/// Releases the read or write lock.

	private:
		xrwlock(const xrwlock&);
		xrwlock& operator = (const xrwlock&);
	};

	/// A variant of xscoped_lock for reader/writer locks.
	class ScopedRWLock
	{
	public:
		ScopedRWLock(xrwlock& rwl, bool write = false);
		~ScopedRWLock();

	private:
		xrwlock& _rwl;

		ScopedRWLock();
		ScopedRWLock(const ScopedRWLock&);
		ScopedRWLock& operator = (const ScopedRWLock&);
	};

	/// A variant of xscoped_lock for reader locks.
	class ScopedReadRWLock : public ScopedRWLock
	{
	public:
		ScopedReadRWLock(xrwlock& rwl);
		~ScopedReadRWLock();
	};


	/// A variant of xscoped_lock for writer locks.
	class ScopedWriteRWLock : public ScopedRWLock
	{
	public:
		ScopedWriteRWLock(xrwlock& rwl);
		~ScopedWriteRWLock();
	};


	//
	// inlines
	//
	inline void xrwlock::readLock()
	{
		readLockImpl();
	}


	inline bool xrwlock::tryReadLock()
	{
		return tryReadLockImpl();
	}


	inline void xrwlock::writeLock()
	{
		writeLockImpl();
	}


	inline bool xrwlock::tryWriteLock()
	{
		return tryWriteLockImpl();
	}


	inline void xrwlock::unlock()
	{
		unlockImpl();
	}


	inline ScopedRWLock::ScopedRWLock(xrwlock& rwl, bool write): _rwl(rwl)
	{
		if (write)
			_rwl.writeLock();
		else
			_rwl.readLock();
	}


	inline ScopedRWLock::~ScopedRWLock()
	{
		_rwl.unlock();
	}


	inline ScopedReadRWLock::ScopedReadRWLock(xrwlock& rwl): ScopedRWLock(rwl, false)
	{
	}


	inline ScopedReadRWLock::~ScopedReadRWLock()
	{
	}


	inline ScopedWriteRWLock::ScopedWriteRWLock(xrwlock& rwl): ScopedRWLock(rwl, true)
	{
	}


	inline ScopedWriteRWLock::~ScopedWriteRWLock()
	{
	}


} // namespace xcore


#endif // __XMTHREAD_RWLOCK_H__
