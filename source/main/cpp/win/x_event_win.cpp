#include "xthread/private/windows/x_event_win.h"

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


	void xevent_impl::event_wait()
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



} // namespace xcore
