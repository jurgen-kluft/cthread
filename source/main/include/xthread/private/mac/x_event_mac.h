#ifndef __XMTHREAD_EVENT_OSX_H__
#define __XMTHREAD_EVENT_OSX_H__
#include "xbase/x_target.h"

#include "xthread/private/osx/x_semaphore_mac.h"

#include <pthread.h>
#include <stdbool.h>

namespace xcore 
{
	class xevent_impl
	{
	protected:
					xevent_impl(bool autoReset);		
					~xevent_impl();

		void		event_set();
		void		event_wait();
		void		event_reset();

	private:
		pthread_mutex_t	_mutex;
		pthread_cond_t	_cond;
		bool			_triggered;
	};

	//
	// inlines
	//
	inline void xevent_impl::event_set()
	{

	}


	inline void xevent_impl::event_reset()
	{

	}


} // namespace xcore


#endif // __XMTHREAD_EVENT_OSX_H__
