#include "xbase/x_target.h"

#ifdef TARGET_MAC
#include "xthread/private/osx/x_event_mac.h"

namespace xcore 
{
	xevent_impl::xevent_impl(bool autoReset)
	{
		pthread_mutex_init(&_mutex, 0);
		pthread_cond_init(&_cond, 0);
		_triggered = false;
	}

	xevent_impl::~xevent_impl()
	{
		
	}


	void xevent_impl::event_wait()
	{
		pthread_mutex_lock(&_mutex);
		while (!_triggered)
			pthread_cond_wait(&_cond, &_mutex);
		pthread_mutex_unlock(&_mutex);
	}


} // namespace xcore

#endif