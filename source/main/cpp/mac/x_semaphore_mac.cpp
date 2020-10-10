#include "xbase/x_target.h"
#include "xbase/x_debug.h"

#ifdef TARGET_MAC

#include "xthread/x_semaphore.h"

namespace xcore 
{
	// Semaphore (Apple iOS and OSX)
	struct xsema_data
	{
		semaphore_t _sema;
	};

	xsemaphore::xsemaphore(s32 n, s32 max)
	{
		ASSERT(n >= 0 && max > 0 && n <= max);
		xsema_data* data = (xsema_data*)m_data;		
		semaphore_create(mach_task_self(), &data->_sema, SYNC_POLICY_FIFO, n);
	}


	xsemaphore::~xsemaphore()
	{
		xsema_data* data = (xsema_data*)m_data;		
		semaphore_destroy(mach_task_self(), data->_sema);
	}

	void xsemaphore::signal()
	{
		xsema_data* data = (xsema_data*)m_data;		
		semaphore_signal(data->_sema);
	}

	void xsemaphore::wait()
	{
		xsema_data* data = (xsema_data*)m_data;		
		semaphore_wait(data->_sema);
	}


} // namespace xcore


#endif