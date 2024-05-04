#include "ccore/c_target.h"

#ifdef TARGET_MAC
#include "cthread/c_event.h"
#include "cthread/private/c_thread_mac.h"

namespace ncore 
{
	bool event_t::init(event_data_t* data, bool autoReset)
	{
		m_data = data;
		pthread_mutex_init(&m_data->_mutex, 0);
		pthread_cond_init(&m_data->_cond, 0);
		m_data->_triggered = false;
		return true;
	}

	void event_t::wait()
	{
		pthread_mutex_lock(&m_data->_mutex);
		while (!m_data->_triggered)
			pthread_cond_wait(&m_data->_cond, &m_data->_mutex);
		pthread_mutex_unlock(&m_data->_mutex);
	}


} // namespace ncore

#endif