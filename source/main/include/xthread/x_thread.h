#ifndef __XMTHREAD_THREAD_H__
#define __XMTHREAD_THREAD_H__
#include "xbase/x_target.h"

#include "xthread/x_mutex.h"

#if defined(TARGET_PC)
#include "xthread/private/windows/x_thread_win.h"
#elif defined(TARGET_MAC)
#include "xthread/private/osx/x_thread_mac.h"
#endif

namespace xcore
{
	class xthread_functor;

	/// This class implements a platform-independent wrapper to an operating system thread.
	///
	/// Every xthread object gets a unique (within its process) numeric thread ID.
	class xthread : public ithread
	{
	public:	
							xthread();							/// Creates a thread. Call start() to start it.
							xthread(const char* name);			/// Creates a named thread. Call start() to start it.
							xthread(config const&);				/// Creates a thread using some configuration. Call start() to start it.

							~xthread();							/// Destroys the thread.

		tid_t				get_tid() const;					/// Returns the native thread ID of the thread.
		idx_t				get_idx() const;					/// Returns the unique thread ID of the thread.

		const char*			get_name() const;					/// Returns the name of the thread.
		void				set_name(const char* name);			/// Sets the name of the thread.

		void				set_priority(e_priority p);			/// Sets the thread's priority.
		e_priority			get_priority() const;				/// Returns the thread's priority.

		e_state				get_state() const;					/// Returns state of the thread

		u32					get_stacksize() const;

		void				start(xthread_functor* f);

		void				join();
		bool				join(u32 milliseconds);

		static xthread*		current();

		static void			sleep(u32 milliseconds);
		static void			yield();
		static void			exit();

	private:
							xthread(const xthread&);
		xthread&			operator = (const xthread&);

		char				m_name[64];
	};


	//
	// inlines
	//
	inline xthread::tid_t xthread::get_tid() const
	{
		return thread_get_tid();
	}


	inline xthread::idx_t xthread::get_idx() const
	{
		return thread_get_idx();
	}

	inline const char* xthread::get_name() const
	{
		return m_name;
	}

	inline xthread::e_state xthread::get_state() const
	{
		return thread_get_state();
	}

	inline u32		xthread::get_stacksize() const
	{
		return thread_get_stacksize();
	}


	inline void xthread::sleep(u32 milliseconds)
	{
		thread_sleep(milliseconds);
	}


	inline void xthread::yield()
	{
		thread_yield();
	}


	inline xthread* xthread::current()
	{
		return static_cast<xthread*>(thread_current());
	}


} // namespace xcore


#endif // __XMTHREAD_THREAD_H__
