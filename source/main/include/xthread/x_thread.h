#ifndef __XMTHREAD_THREAD_H__
#define __XMTHREAD_THREAD_H__
#include "xbase/x_target.h"

#include "xthread/x_mutex.h"

#if defined(TARGET_PC)
#include "xthread/private/windows/x_thread_win.h"
#elif defined(TARGET_OSX)
#include "xthread/private/osx/x_thread_osx.h"
#endif

namespace xcore
{
	class xthread_functor;

	/// This class implements a platform-independent wrapper to an operating system thread.
	///
	/// Every xthread object gets a unique (within its process) numeric thread ID.
	class xthread : private xthread_impl
	{
	public:	
							xthread();							/// Creates a thread. Call start() to start it.
							xthread(const char* name);			/// Creates a named thread. Call start() to start it.

							~xthread();							/// Destroys the thread.

		xthread_idx_t		get_idx() const;					/// Returns the unique thread ID of the thread.
		xthread_tid_t		get_tid() const;					/// Returns the native thread ID of the thread.

		const char*			get_name() const;					/// Returns the name of the thread.
		void				set_name(const char* name);			/// Sets the name of the thread.

		void				set_priority(EThreadPriority p);	/// Sets the thread's priority.
		EThreadPriority		get_priority() const;				/// Returns the thread's priority.

		void				start(xthread_functor* f);

		void				join();
		void				join(u32 milliseconds);

		enum EState
		{
			STATE_CREATED  = 0,
			STATE_RUNNING  = 1,
			STATE_STOPPED  = 2
		};
		EState				get_state() const;					/// Returns state of the thread

		static xthread*		current();

		static void			sleep(u32 milliseconds);
		static void			yield();
		static void			exit();

	private:
							xthread(const xthread&);
		xthread&			operator = (const xthread&);

		u64					_id;
		char				_name[64];
	};


	//
	// inlines
	//
	inline thread_tid_t xthread::get_tid() const
	{
		return tidImpl();
	}


	inline thread_idx_t xthread::get_idx() const
	{
		return _id;
	}

	inline const char* xthread::get_name() const
	{
		return _name;
	}

	inline bool xthread::get_state() const
	{
		return isRunningImpl();
	}

	inline void xthread::sleep(u32 milliseconds)
	{
		sleepImpl(milliseconds);
	}


	inline void xthread::yield()
	{
		yieldImpl();
	}


	inline xthread* xthread::current()
	{
		return static_cast<xthread*>(currentImpl());
	}


	inline void xthread::set_stack_size(s32 size)
	{
		setStackSizeImpl(size);
	}


	inline s32 xthread::get_stack_size() const
	{
		return getStackSizeImpl();
	}


} // namespace xcore


#endif // __XMTHREAD_THREAD_H__
