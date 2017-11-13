#ifndef __XMTHREAD_SCOPEDLOCK_H__
#define __XMTHREAD_SCOPEDLOCK_H__

#include "xbase\x_target.h"

namespace xcore
{
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a xmutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
	template <class M>
	class xscoped_lock
	{
	public:
		explicit	xscoped_lock(M& mutex)
						: _mutex(mutex)								{ _mutex.lock(); }
					xscoped_lock(M& mutex, u32 milliseconds)
						: _mutex(mutex)								{ _mutex.lock(milliseconds); }
					~xscoped_lock()									{ _mutex.unlock(); }

	private:
		M&			_mutex;

					xscoped_lock();
					xscoped_lock(const xscoped_lock&);

		xscoped_lock& operator = (const xscoped_lock&);
	};

	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a xmutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
	/// The unlock() member function allows for manual
	/// unlocking of the mutex.
	template <class M>
	class xscopedlock_with_unlock
	{
	public:
		explicit	xscopedlock_with_unlock(M& mutex)
						: _pMutex(&mutex)							{ _pMutex->lock(); }
					xscopedlock_with_unlock(M& mutex, u32 milliseconds)
						: _pMutex(&mutex)							{ _pMutex->lock(milliseconds); }

					~xscopedlock_with_unlock()						{ unlock(); }

		void		unlock()
		{
			if (_pMutex)
			{
				_pMutex->unlock();
				_pMutex = 0;
			}
		}

	private:
		M*			_pMutex;

					xscopedlock_with_unlock();
					xscopedlock_with_unlock(const xscopedlock_with_unlock&);

		xscopedlock_with_unlock& operator = (const xscopedlock_with_unlock&);
	};


} // namespace xcore


#endif // __XMTHREAD_SCOPEDLOCK_H__
