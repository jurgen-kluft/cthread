#include "ccore/c_target.h"

#ifdef TARGET_PC
#include "cthread/c_event.h"
#include "cthread/c_threading.h"
#include "cthread/private/c_thread_win.h"

#include <Windows.h>

namespace ncore 
{
	bool event_t::init(event_data_t* data, bool autoReset)
	{
		m_data = data;
		m_data->m_event = CreateEventW(nullptr, autoReset ? FALSE : TRUE, FALSE, nullptr);
		if (!m_data->m_event)
		{
			// cannot create event
			return false;
		}
		return true;
	}

	void event_t::release()
	{
		CloseHandle(m_data->m_event);
		threading_t::instance()->destroy(this);
	}

	void event_t::wait()
	{
		switch (WaitForSingleObject(m_data->m_event, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			// wait for event failed
			break;
		}
	}

	void event_t::set()
	{
		if (!SetEvent(m_data->m_event))
		{
			// cannot signal event
		}
	}


	void event_t::reset()
	{
		if (!ResetEvent(m_data->m_event))
		{
			// cannot reset event
		}
	}


} // namespace ncore

#endif