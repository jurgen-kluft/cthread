#include "ccore/c_target.h"

#if defined(TARGET_TEST)
#include "cthread/c_event.h"
#include "cthread/c_threading.h"
#include "cthread/private/c_thread_mock.h"

namespace ncore 
{
	bool event_t::init(event_data_t* data, bool autoReset)
	{
		m_data = data;
        m_data->m_event = 1;
		return true;
	}

	void event_t::release()
	{
		threading_t::instance()->destroy(this);
	}

	void event_t::wait()
	{
		
	}

	void event_t::set()
	{

	}


	void event_t::reset()
	{

	}


} // namespace ncore

#endif