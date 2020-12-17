#include "xbase/x_target.h"

#ifdef TARGET_PC
#include "xthread/x_event.h"
#include "xthread/x_threading.h"
#include "xthread/private/x_threading.h"

#include <Windows.h>

namespace xcore 
{
	class xevent_win : public xevent
	{
	public:
		xevent_win(bool autoReset) : xevent(autoReset) {  }

		XCORE_CLASS_PLACEMENT_NEW_DELETE

		void* m_event;
	};

	xevent::xevent(bool autoReset)
	{
		xevent_win* the = (xevent_win*)this;
		the->m_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
		if (!the->m_event)
		{
			// cannot create event
		}
	}

	xevent::~xevent()
	{
		xevent_win* the = (xevent_win*)this;
		CloseHandle(the->m_event);
	}


	void xevent::wait()
	{
		xevent_win* the = (xevent_win*)this;
		switch (WaitForSingleObject(the->m_event, INFINITE))
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
		xevent_win* the = (xevent_win*)this;
		if (!SetEvent(the->m_event))
		{
			// cannot signal event
		}
	}


	void xevent::reset()
	{
		xevent_win* the = (xevent_win*)this;
		if (!ResetEvent(the->m_event))
		{
			// cannot reset event
		}
	}

	class xevents_data
	{
	public:
		xevent*         m_events;
		fsadexed_array_t m_alloc;
		XCORE_CLASS_PLACEMENT_NEW_DELETE
	};

	xevents_data*	gCreateEventsData(alloc_t* alloc, u32 max_events)
	{
		xevents_data* events = alloc->construct<xevents_data>();
		xevent_win* event_array = (xevent_win*)alloc->allocate(sizeof(xevent_win) * max_events);
		events->m_alloc = fsadexed_array_t(event_array, sizeof(xevent_win), max_events);
		return events;
	}

	xevent* xthreading::create_event(bool autoReset)
	{
		xevent_win* evt = m_events->m_alloc.construct<xevent_win>(autoReset);
		return evt;
	}

	void xthreading::destroy_event(xevent* evt)
	{
		xevent_win* win_evt = (xevent_win*)evt;
		m_events->m_alloc.destruct(win_evt);
	}


} // namespace xcore

#endif