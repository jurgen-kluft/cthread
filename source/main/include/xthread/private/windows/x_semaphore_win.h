#ifndef __XMTHREAD_SEMAPHORE_WIN32_H__
#define __XMTHREAD_SEMAPHORE_WIN32_H__
#include "xbase/x_target.h"

#include <Windows.h>

namespace xcore 
{
	class xsemaphore_impl
	{
	protected:
				xsemaphore_impl(s32 n, s32 max);		
				~xsemaphore_impl();

		void	sema_signal();
		void	sema_wait();

	private:
		HANDLE	_sema;
	};


	//
	// inlines
	//
	inline void xsemaphore_impl::sema_signal()
	{
		if (!ReleaseSemaphore(_sema, 1, NULL))
		{
			// cannot signal semaphore
		}
	}


} // namespace xcore


#endif // __XMTHREAD_SEMAPHORE_WIN32_H__
