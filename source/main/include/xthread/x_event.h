#ifndef __XMTHREAD_EVENT_H__
#define __XMTHREAD_EVENT_H__
#include "xbase/x_target.h"

namespace xcore 
{
	#if defined(TARGET_PC)
		const int xevent_data_size64 = 1;
	#elif defined(TARGET_MAC)
		const int xevent_data_size64 = 10;
	#else
		const int xevent_data_size64 = -1;
	#endif	
	// An xevent is a synchronization object that allows one thread to signal one or more
	// other threads that a certain event has happened.
	// Usually, one thread signals an event, while one or more other threads wait
	// for an event to become signalled.
	class xevent
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
		u64			m_data[xevent_data_size64];
					xevent(const xevent&);
					xevent& operator = (const xevent&);
	};


} // namespace xcore


#endif // __XMTHREAD_EVENT_H__
