#include "xbase/x_target.h"

#ifdef TARGET_MAC
#include "xthread/x_event.h"

namespace ncore 
{
	struct xevent_data
	{
		pthread_mutex_t	_mutex;
		pthread_cond_t	_cond;
		bool			_triggered;
	};

	xevent::xevent(bool autoReset)
	{
		xevent_data* data = (xevent_data*)m_data;
		pthread_mutex_init(&data->_mutex, 0);
		pthread_cond_init(&data->_cond, 0);
		data->_triggered = false;
	}

	xevent::~xevent()
	{
		
	}


	void xevent_impl::wait()
	{
		xevent_data* data = (xevent_data*)m_data;
		pthread_mutex_lock(&data->_mutex);
		while (!data->_triggered)
			pthread_cond_wait(&data->_cond, &_mutex);
		pthread_mutex_unlock(&data->_mutex);
	}


} // namespace ncore

#endif