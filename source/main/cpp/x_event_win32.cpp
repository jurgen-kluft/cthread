#include "xmthread\private\x_event_win32.h"

namespace xcore 
{
	xevent_impl::xevent_impl(bool autoReset)
	{
		_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
		if (!_event)
		{
			// cannot create event
		}
	}

	xevent_impl::~xevent_impl()
	{
		CloseHandle(_event);
	}


	void xevent_impl::waitImpl()
	{
		switch (WaitForSingleObject(_event, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			// wait for event failed
			break;
		}
	}

	bool xevent_impl::waitImpl(u32 milliseconds)
	{
		switch (WaitForSingleObject(_event, milliseconds + 1))
		{
		case WAIT_TIMEOUT:
			return false;
		case WAIT_OBJECT_0:
			return true;
		default:
			// wait for event failed
			return false;
		}
	}


} // namespace xcore
