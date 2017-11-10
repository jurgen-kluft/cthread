#ifndef __XMTHREAD_NAMEDEVENT_H__
#define __XMTHREAD_NAMEDEVENT_H__
#include "xbase\x_target.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_namedevent_win32.h"
#endif

namespace xcore
{
	/// An xnevent is a global synchronization object 
	/// that allows one process or thread to signal an
	/// other process or thread that a certain event
	/// has happened.
	///
	/// Unlike an xevent, which itself is the unit of synchronization,
	/// a xnevent refers to a named operating system resource being the
	/// unit of synchronization.
	/// In other words, there can be multiple instances of xnevent referring
	/// to the same actual synchronization object.
	///
	/// NamedEvents are always autoresetting.
	///
	/// There should not be more than one instance of xnevent for
	/// a given name in a process. Otherwise, the instances may
	/// interfere with each other.
	class xnevent : public xnevent_impl
	{
	public:
					xnevent(const char* name);
		/// Creates the event.

					~xnevent();
		/// Destroys the event.

		void		set();
		/// Signals the event. 
		/// The one thread or process waiting for the event 
		/// can resume execution.

		void		wait();
		/// Waits for the event to become signalled.

	private:
					xnevent();
					xnevent(const xnevent&);

		xnevent&	operator = (const xnevent&);
	};


	//
	// inlines
	//
	inline void xnevent::set()
	{
		setImpl();
	}


	inline void xnevent::wait()
	{
		waitImpl();
	}


} // namespace xcore


#endif // __XMTHREAD_NAMEDEVENT_H__
