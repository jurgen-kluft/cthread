#ifndef __XMTHREAD_EVENT_H__
#define __XMTHREAD_EVENT_H__
#include "xbase\x_target.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_event_win32.h"
#endif


namespace xcore 
{
	/// An xevent is a synchronization object that
	/// allows one thread to signal one or more
	/// other threads that a certain event
	/// has happened.
	/// Usually, one thread signals an event,
	/// while one or more other threads wait
	/// for an event to become signalled.
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

		void		wait(u32 milliseconds);
		/// Waits for the event to become signalled.
		/// Throws a TimeoutException if the event
		/// does not become signalled within the specified
		/// time interval.

		bool		tryWait(u32 milliseconds);
		/// Waits for the event to become signalled.
		/// Returns true if the event
		/// became signalled within the specified
		/// time interval, false otherwise.

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
		setImpl();
	}


	inline void xevent::wait()
	{
		waitImpl();
	}


	inline void xevent::wait(u32 milliseconds)
	{
		waitImpl(milliseconds);
	}


	inline bool xevent::tryWait(u32 milliseconds)
	{
		return waitImpl(milliseconds);
	}


	inline void xevent::reset()
	{
		resetImpl();
	}


} // namespace xcore


#endif // __XMTHREAD_EVENT_H__
