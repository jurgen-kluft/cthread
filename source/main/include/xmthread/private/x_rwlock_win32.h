#ifndef __XMTHREAD_RWLOCK_WIN32_H__
#define __XMTHREAD_RWLOCK_WIN32_H__
#include "xbase\x_target.h"

#include <Windows.h>

namespace xcore 
{
	class xrwlock_impl
	{
	protected:
					xrwlock_impl();
					~xrwlock_impl();

		void		readLockImpl();
		bool		tryReadLockImpl();
		void		writeLockImpl();
		bool		tryWriteLockImpl();
		void		unlockImpl();

	private:
		void		addWriter();
		void		removeWriter();
		DWORD		tryReadLockOnce();

		HANDLE		_mutex;
		HANDLE		_readEvent;
		HANDLE		_writeEvent;
		u32			_readers;
		u32			_writersWaiting;
		u32			_writers;
	};


} // namespace xcore


#endif // __XMTHREAD_RWLOCK_WIN32_H__
