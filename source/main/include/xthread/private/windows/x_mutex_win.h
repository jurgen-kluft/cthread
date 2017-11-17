#ifndef __XMTHREAD_MUTEX_WIN32_H__
#define __XMTHREAD_MUTEX_WIN32_H__
#include "xbase/x_target.h"

#include <Windows.h>

namespace xcore 
{
	class xmutex_impl
	{
	protected:
							xmutex_impl();
							~xmutex_impl();

		void				mutex_lock();
		bool				mutex_tryLock();
		void				mutex_unlock();

	private:
		CRITICAL_SECTION	_cs;
	};

	//
	// inlines
	//
	inline void xmutex_impl::mutex_lock()
	{
		EnterCriticalSection(&_cs);
	}


	inline bool xmutex_impl::mutex_tryLock()
	{
		return TryEnterCriticalSection(&_cs) != 0;
	}


	inline void xmutex_impl::mutex_unlock()
	{
		LeaveCriticalSection(&_cs);
	}

} // namespace xcore


#endif // __XMTHREAD_MUTEX_WIN32_H__
