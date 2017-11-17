#include "xthread/x_semaphore.h"

namespace xcore 
{
	xsemaphore::xsemaphore(s32 n)
		: xsemaphore_impl(n, n)
	{
	}

	xsemaphore::xsemaphore(s32 n, s32 max)
		: xsemaphore_impl(n, max)
	{
	}

	xsemaphore::~xsemaphore()
	{
	}


} // namespace xcore
