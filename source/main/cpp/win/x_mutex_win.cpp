#include "xbase/x_target.h"
#include "xtime/x_datetime.h"

#ifdef TARGET_PC
#include "xthread/private/windows/x_mutex_win.h"
#include <Windows.h>

namespace xcore
{
	struct xmutex_data
	{
		CRITICAL_SECTION _cs;
	};

	xmutex::xmutex()
	{
		xmutex_data* data = (xmutex_data*)m_data;

		// the func has a boolean return value under WInnNt/2000/XP but not on Win98
		// the return only checks if the input address of &_cs was valid, so it is 
		// safe to omit it.
		InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION*)&data->_cs, 4000);
	}

	xmutex::~xmutex()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		DeleteCriticalSection((CRITICAL_SECTION*)&data->_cs);
	}

	//
	// inlines
	//
	void xmutex::lock()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		EnterCriticalSection((CRITICAL_SECTION*)&data->_cs);
	}


	bool xmutex::tryLock()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		return TryEnterCriticalSection((CRITICAL_SECTION*)&data->_cs) != 0;
	}


	void xmutex::unlock()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		LeaveCriticalSection((CRITICAL_SECTION*)&data->_cs);
	}

} // namespace xcore

#endif