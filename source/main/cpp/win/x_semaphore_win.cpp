#include "xbase/x_target.h"
#include "xbase/x_debug.h"

#ifdef TARGET_PC
#include "xthread/private/windows/x_semaphore_win.h"
#include <Windows.h>

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

	//
	// inlines
	//
	void xsemaphore_impl::sema_signal()
	{
		if (!ReleaseSemaphore(_sema, 1, NULL))
		{
			// cannot signal semaphore
		}
	}

	void xsemaphore_impl::sema_wait()
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

	bool	xsemaphore_impl::sema_try_wait(u32 milliseconds)
	{
		switch (WaitForSingleObject(_sema, milliseconds))
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			// wait for semaphore failed
			break;
		}
		return false;
	}


} // namespace xcore

#endif