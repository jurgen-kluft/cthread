#ifndef __XMTHREAD_EVENT_H__
#define __XMTHREAD_EVENT_H__
#include "xbase/x_target.h"

#if defined(TARGET_PC)
#include "xthread/private/windows/x_event_win.h"
#elif defined(TARGET_OSX)
#include "xthread/private/osx/x_event_osx.h"
#endif


namespace xcore 
{
	// An xevent is a synchronization object that allows one thread to signal one or more
	// other threads that a certain event has happened.
	// Usually, one thread signals an event, while one or more other threads wait
	// for an event to become signalled.
	class xevent : private xevent_impl
	{
	public:
					xevent(bool autoReset = true);
		/// Creates the event. If autoReset is true,
		/// the event is automatically reset after
		/// a wait() successfully returns.

					~xevent();
		/// Destroys the event.

		void		set();
		/// Signals the event. If autoReset is true,
		/// only one thread waiting for the event 
		/// can resume execution.
		/// If autoReset is false, all waiting threads
		/// can resume execution.

		void		wait();
		/// Waits for the event to become signalled.

		void		reset();
		/// Resets the event to unsignalled state.

	private:
					xevent(const xevent&);
					xevent& operator = (const xevent&);
	};


	//
	// inlines
	//
	inline void xevent::set()
	{
		event_set();
	}


	inline void xevent::wait()
	{
		event_wait();
	}

	inline void xevent::reset()
	{
		event_reset();
	}


} // namespace xcore


#endif // __XMTHREAD_EVENT_H__
