#ifndef __XMTHREAD_SEMAPHORE_WIN32_H__
#define __XMTHREAD_SEMAPHORE_WIN32_H__
#include "xbase/x_target.h"


namespace xcore 
{
	class xsemaphore_impl
	{
	protected:
				xsemaphore_impl(s32 n, s32 max);		
				~xsemaphore_impl();

		void	sema_signal();
		void	sema_wait();
		bool	sema_try_wait(u32 milliseconds);

	private:
		void*	_sema;
	};


} // namespace xcore


#endif // __XMTHREAD_SEMAPHORE_WIN32_H__
