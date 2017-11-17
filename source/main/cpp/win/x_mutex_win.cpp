#include "xthread/private/windows/x_mutex_win.h"
#include "xtime/x_datetime.h"

namespace xcore
{
	xmutex_impl::xmutex_impl()
	{
		// the func has a boolean return value under WInnNt/2000/XP but not on Win98
		// the return only checks if the input address of &_cs was valid, so it is 
		// safe to omit it.
		InitializeCriticalSectionAndSpinCount(&_cs, 4000);
	}

	xmutex_impl::~xmutex_impl()
	{
		DeleteCriticalSection(&_cs);
	}


} // namespace xcore
