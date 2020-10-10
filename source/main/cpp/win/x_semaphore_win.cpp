#include "xbase/x_target.h"
#include "xbase/x_debug.h"

#ifdef TARGET_PC
#include "xthread/x_semaphore.h"
#include <Windows.h>

namespace xcore 
{
	struct xsema_data
	{
		void* _sema;
	};

	xsemaphore::xsemaphore(s32 n, s32 max)
	{
		ASSERT(n >= 0 && max > 0 && n <= max);
		xsema_data* data = (xsema_data*)m_data;
		data->_sema = CreateSemaphoreW(NULL, n, max, NULL);
		if (!data->_sema)
		{
			// cannot create semaphore
		}
	}

	xsemaphore::~xsemaphore()
	{
		xsema_data* data = (xsema_data*)m_data;
		CloseHandle(data->_sema);
	}

	//
	// inlines
	//
	void xsemaphore::signal()
	{
		xsema_data* data = (xsema_data*)m_data;
		if (!ReleaseSemaphore(data->_sema, 1, NULL))
		{
			// cannot signal semaphore
		}
	}

	void xsemaphore::wait()
	{
		xsema_data* data = (xsema_data*)m_data;
		switch (WaitForSingleObject(data->_sema, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			// wait for semaphore failed
			break;
		}
	}

	bool	xsemaphore::try_wait(u32 milliseconds)
	{
		xsema_data* data = (xsema_data*)m_data;
		switch (WaitForSingleObject(data->_sema, milliseconds))
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