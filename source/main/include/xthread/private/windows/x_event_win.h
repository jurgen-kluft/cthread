#ifndef __XMTHREAD_EVENT_WIN32_H__
#define __XMTHREAD_EVENT_WIN32_H__
#include "xbase/x_target.h"


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
		void *		_event;
	};



} // namespace xcore


#endif // __XMTHREAD_EVENT_WIN32_H__
