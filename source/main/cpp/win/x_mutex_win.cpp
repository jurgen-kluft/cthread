#include "xbase/x_target.h"
#include "xtime/x_datetime.h"

#ifdef TARGET_PC
#include "xthread/private/windows/x_mutex_win.h"
#include <Windows.h>

namespace xcore
{
	xmutex_impl::xmutex_impl()
	{
		// the func has a boolean return value under WInnNt/2000/XP but not on Win98
		// the return only checks if the input address of &_cs was valid, so it is 
		// safe to omit it.
		InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION*)&_cs, 4000);
	}

	xmutex_impl::~xmutex_impl()
	{
		DeleteCriticalSection((CRITICAL_SECTION*)&_cs);
	}

	//
	// inlines
	//
	void xmutex_impl::mutex_lock()
	{
		EnterCriticalSection((CRITICAL_SECTION*)&_cs);
	}


	bool xmutex_impl::mutex_tryLock()
	{
		return TryEnterCriticalSection((CRITICAL_SECTION*)&_cs) != 0;
	}


	void xmutex_impl::mutex_unlock()
	{
		LeaveCriticalSection((CRITICAL_SECTION*)&_cs);
	}

} // namespace xcore

#endif