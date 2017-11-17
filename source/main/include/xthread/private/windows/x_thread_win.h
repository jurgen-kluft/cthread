#ifndef __XMTHREAD_THREAD_WIN32_H__
#define __XMTHREAD_THREAD_WIN32_H__
#include "xbase/x_target.h"

#include <Windows.h>

namespace xcore
{
	class xthread_functor;

#if defined(TARGET_DEVKIT)
	#define XTHREAD_DEBUGGER_THREAD_NAMES
#endif

	enum
	{
		XTHREAD_DEFAULT_STACK_SIZE = 8192
	};

	typedef		s32			xthread_idx_t;
	typedef		u64			xthread_tid_t;
	typedef		HANDLE		xthread_hnd_t;

	enum EThreadPriority
	{
		XTHREAD_PRIORITY_LOWEST = THREAD_PRIORITY_LOWEST,
		XTHREAD_PRIORITY_LOW = THREAD_PRIORITY_BELOW_NORMAL,
		XTHREAD_PRIORITY_NORMAL = THREAD_PRIORITY_NORMAL,
		XTHREAD_PRIORITY_HIGH = THREAD_PRIORITY_ABOVE_NORMAL,
		XTHREAD_PRIORITY_HIGHEST = THREAD_PRIORITY_HIGHEST
	};
	enum EThreadState
	{
		XTHREAD_STATE_SETUP = 1,
		XTHREAD_STATE_CREATED = 100,
		XTHREAD_STATE_RUNNING = 200,
		XTHREAD_STATE_STOPPED = 300
	};

	struct xthread_config
	{
		xthread_config()
			: m_priority(XTHREAD_PRIORITY_NORMAL)
			, m_stack_size(XTHREAD_DEFAULT_STACK_SIZE)
		{}

		EThreadPriority		m_priority;
		u32					m_stack_size;
	};

	class xthread_impl
	{
	protected:
						xthread_impl(xthread_config const& _config);
						~xthread_impl();

		xthread_idx_t	thread_get_idx() const;
		xthread_tid_t	thread_get_tid() const;

		void			thread_set_priority(EThreadPriority prio);
		EThreadPriority	thread_get_priority() const;

		void			thread_start(xthread_functor* _f);
		void			thread_run();
		void			thread_exit();

		void			thread_join();
		bool			thread_join(u32 milliseconds);

		EThreadState	thread_get_state() const;

		static xthread_impl*	thread_current();
		static xthread_tid_t	thread_current_tid();
		static xthread_idx_t	thread_current_idx();

		static void		thread_create(xthread_impl* _this);
		static void		thread_sleep(u32 milliseconds);
		static void		thread_yield();

	protected:
		friend unsigned __stdcall 	__main_func(void* arg);

		xthread_hnd_t		m_threadHandle;
		xthread_idx_t		m_threadIdx;
		xthread_tid_t		m_threadTid;
		EThreadState		m_threadState;
		xthread_functor*	m_functor;
		xthread_config		m_config;
	};


	//
	// inlines
	//
	inline xthread_idx_t xthread_impl::thread_get_idx() const
	{
		return m_threadIdx;
	}

	inline xthread_tid_t xthread_impl::thread_get_tid() const
	{
		return m_threadTid;
	}

	inline EThreadPriority xthread_impl::thread_get_priority() const
	{
		return m_config.m_priority;
	}

	inline void xthread_impl::thread_sleep(u32 milliseconds)
	{
		::Sleep(DWORD(milliseconds));
	}


	inline void xthread_impl::thread_yield()
	{
		::Sleep(0);
	}

} // namespace xcore


#endif // __XMTHREAD_THREAD_WIN32_H__
