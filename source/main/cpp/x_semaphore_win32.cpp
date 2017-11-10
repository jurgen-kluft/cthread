#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xmthread\private\x_semaphore_win32.h"

namespace xcore 
{
	xsemaphore_impl::xsemaphore_impl(s32 n, s32 max)
	{
		ASSERT(n >= 0 && max > 0 && n <= max);

		_sema = CreateSemaphoreW(NULL, n, max, NULL);
		if (!_sema)
		{
			// cannot create semaphore
		}
	}


	xsemaphore_impl::~xsemaphore_impl()
	{
		CloseHandle(_sema);
	}


	void xsemaphore_impl::waitImpl()
	{
		switch (WaitForSingleObject(_sema, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			// wait for semaphore failed
			break;
		}
	}


	bool xsemaphore_impl::waitImpl(u32 milliseconds)
	{
		switch (WaitForSingleObject(_sema, milliseconds + 1))
		{
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			return true;
		default:
			// wait for semaphore failed
			return false;
		}
	}


} // namespace xcore
