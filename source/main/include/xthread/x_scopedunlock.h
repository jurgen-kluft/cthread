#ifndef __XMTHREAD_SCOPEDUNLOCK_H__
#define __XMTHREAD_SCOPEDUNLOCK_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a xmutex and unlocks it.
	/// The destructor locks the mutex.
	template <class M>
	class xscopedunlock
	{
	public:
		inline xscopedunlock(M& mutex, bool unlockNow = true)
			: _mutex(mutex)
		{
			if (unlockNow)
				_mutex.unlock();
		}

		inline	~xscopedunlock()		{ _mutex.lock(); }

	private:
		M&		_mutex;

				xscopedunlock();
				xscopedunlock(const xscopedunlock&);

		xscopedunlock& operator = (const xscopedunlock&);
	};


} // namespace ncore


#endif // __XMTHREAD_SCOPEDUNLOCK_H__
