#ifndef __XMTHREAD_SCOPEDLOCK_H__
#define __XMTHREAD_SCOPEDLOCK_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a xmutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
	template <class M>
	class xlock
	{
	public:
		explicit	xlock(M& primitive)
						: _primitive(primitive)						{ _primitive.lock(); }
					xlock(M& _primitive, u32 milliseconds)
						: _primitive(primitive)						{ _primitive.lock(milliseconds); }
					~xlock()										{ _primitive.unlock(); }

	private:
		M&			_primitive;

					xlock();
					xlock(const xlock&);

		xlock&		operator = (const xlock&);
	};


} // namespace ncore


#endif // __XMTHREAD_SCOPEDLOCK_H__
