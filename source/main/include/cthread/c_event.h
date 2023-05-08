#ifndef __CMTHREAD_EVENT_H__
#define __CMTHREAD_EVENT_H__
#include "ccore/c_target.h"

namespace ncore 
{

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
					xevent(const xevent&);
					xevent& operator = (const xevent&);
	};


} // namespace ncore


#endif // __CMTHREAD_EVENT_H__
