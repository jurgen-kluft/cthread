#include "xmthread\private\x_namedevent_win32.h"

namespace xcore
{
	xnevent_impl::xnevent_impl(const char* name)
	{
		_event = CreateEventA(NULL, FALSE, FALSE, name);
		if (!_event)
		{
			// cannot create named event
		}
	}


	xnevent_impl::~xnevent_impl()
	{
		CloseHandle(_event);
	}


	void xnevent_impl::setImpl()
	{
		if (!SetEvent(_event))
		{
			// cannot signal named event
		}
	}


	void xnevent_impl::waitImpl()
	{
		switch (WaitForSingleObject(_event, INFINITE))
		{
		case WAIT_OBJECT_0:
			break;
		default:
			// wait for named event failed
			break;
		}
	}


} // namespace xcore
