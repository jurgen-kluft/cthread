#ifndef __XMTHREAD_MUTEX_WIN32_H__
#define __XMTHREAD_MUTEX_WIN32_H__
#include "xbase\x_target.h"

#include <Windows.h>

namespace xcore 
{
	class xmutex_impl
	{
	protected:
							xmutex_impl();
							~xmutex_impl();

		void				lockImpl();
		bool				tryLockImpl();
		bool				tryLockImpl(u32 milliseconds);
		void				unlockImpl();

	private:
		CRITICAL_SECTION	_cs;
	};

	typedef xmutex_impl FastMutexImpl;

	//
	// inlines
	//
	inline void xmutex_impl::lockImpl()
	{
		EnterCriticalSection(&_cs);
	}


	inline bool xmutex_impl::tryLockImpl()
	{
		return TryEnterCriticalSection(&_cs) != 0;
	}


	inline void xmutex_impl::unlockImpl()
	{
		LeaveCriticalSection(&_cs);
	}

} // namespace xcore


#endif // __XMTHREAD_MUTEX_WIN32_H__
