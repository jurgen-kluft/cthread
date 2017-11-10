#include "xmthread\private\x_mutex_win32.h"
#include "xtime\x_datetime.h"

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

	bool xmutex_impl::tryLockImpl(u32 milliseconds)
	{
		u32 sleepMillis = milliseconds / 4;
		if (sleepMillis == 0) sleepMillis = 1;

		xdatetime mark = xdatetime::sNow();
		mark.addMilliseconds(milliseconds);
		do
		{
			if (TryEnterCriticalSection(&_cs) == TRUE)
				return true;

			Sleep(sleepMillis);

			// Have we exhausted (passed) the time-out?
			xdatetime now = xdatetime::sNow();
			if (now >= mark)
				break;

		} while (true);
		return false;
	}


} // namespace xcore
