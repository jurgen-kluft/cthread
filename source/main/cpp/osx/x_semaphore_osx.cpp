#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xthread/private/osx/x_semaphore_osx.h"

namespace xcore 
{
	xsemaphore_impl::xsemaphore_impl(s32 n, s32 max)
	{
		ASSERT(n >= 0 && max > 0 && n <= max);
		semaphore_create(mach_task_self(), &_sema, SYNC_POLICY_FIFO, n);
	}


	xsemaphore_impl::~xsemaphore_impl()
	{
		semaphore_destroy(mach_task_self(), _sema);
	}


	void xsemaphore_impl::sema_wait()
	{
		semaphore_wait(_sema);
	}


} // namespace xcore
