#include "xbase/x_target.h"

#ifdef TARGET_PC
#include "xthread/x_event.h"
#include <Windows.h>

namespace xcore 
{
	struct xevent_data
	{
		void * _event;
	};

	xevent::xevent(bool autoReset)
	{
		xevent_data* data = (xevent_data*)m_data;
		data->_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
		if (!data->_event)
		{
			// cannot create event
		}
	}

	xevent::~xevent()
	{
		xevent_data* data = (xevent_data*)m_data;
		CloseHandle(data->_event);
	}


	void xevent::wait()
	{
		xevent_data* data = (xevent_data*)m_data;
		switch (WaitForSingleObject(data->_event, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			// wait for event failed
			break;
		}
	}

	void xevent::set()
	{
		xevent_data* data = (xevent_data*)m_data;
		if (!SetEvent(data->_event))
		{
			// cannot signal event
		}
	}


	void xevent::reset()
	{
		xevent_data* data = (xevent_data*)m_data;
		if (!ResetEvent(data->_event))
		{
			// cannot reset event
		}
	}


} // namespace xcore

#endif