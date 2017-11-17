#ifndef __XMTHREAD_SEMAPHORE_OSX_H__
#define __XMTHREAD_SEMAPHORE_OSX_H__
#include "xbase/x_target.h"

#include <mach/mach.h>

namespace xcore 
{
	// Semaphore (Apple iOS and OSX)
	class xsemaphore_impl
	{
	protected:
				xsemaphore_impl(s32 n, s32 max);		
				~xsemaphore_impl();

		void	sema_signal();
		void	sema_wait();

	private:
		semaphore_t _sema;
	};


	//
	// inlines
	//
	inline void xsemaphore_impl::sema_signal()
	{
		semaphore_signal(_sema);
	}


} // namespace xcore


#endif // __XMTHREAD_SEMAPHORE_OSX_H__
