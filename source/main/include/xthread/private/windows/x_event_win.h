#ifndef __XMTHREAD_EVENT_WIN32_H__
#define __XMTHREAD_EVENT_WIN32_H__
#include "xbase/x_target.h"

#include <Windows.h>

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
		HANDLE		_event;
	};

	//
	// inlines
	//
	inline void xevent_impl::event_set()
	{
		if (!SetEvent(_event))
		{
			// cannot signal event
		}
	}


	inline void xevent_impl::event_reset()
	{
		if (!ResetEvent(_event))
		{
			// cannot reset event
		}
	}


} // namespace xcore


#endif // __XMTHREAD_EVENT_WIN32_H__
