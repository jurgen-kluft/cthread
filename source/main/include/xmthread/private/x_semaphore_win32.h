#ifndef __XMTHREAD_SEMAPHORE_WIN32_H__
#define __XMTHREAD_SEMAPHORE_WIN32_H__
#include "xbase\x_target.h"

#include <Windows.h>

namespace xcore 
{
	class xsemaphore_impl
	{
	protected:
				xsemaphore_impl(s32 n, s32 max);		
				~xsemaphore_impl();

		void	setImpl();
		void	waitImpl();
		bool	waitImpl(u32 milliseconds);

	private:
		HANDLE	_sema;
	};


	//
	// inlines
	//
	inline void xsemaphore_impl::setImpl()
	{
		if (!ReleaseSemaphore(_sema, 1, NULL))
		{
			// cannot signal semaphore
		}
	}


} // namespace xcore


#endif // __XMTHREAD_SEMAPHORE_WIN32_H__
