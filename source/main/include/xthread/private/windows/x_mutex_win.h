#ifndef __XMTHREAD_MUTEX_WIN32_H__
#define __XMTHREAD_MUTEX_WIN32_H__
#include "xbase/x_target.h"


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
		u64					_cs[8];
	};


} // namespace xcore


#endif // __XMTHREAD_MUTEX_WIN32_H__
