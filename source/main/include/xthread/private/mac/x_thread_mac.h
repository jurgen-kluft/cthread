#ifndef __XMTHREAD_THREAD_OSX_H__
#define __XMTHREAD_THREAD_OSX_H__
#include "xbase/x_target.h"

#include "xthread/x_thread_functor.h"

#include <thread>
#include <chrono>

namespace ncore
{
#if defined(TARGET_DEVKIT)
	#define XTHREAD_DEBUGGER_THREAD_NAMES
#endif
	enum
	{
		XTHREAD_DEFAULT_STACK_SIZE = 8192
	};

	typedef		s32			xthread_idx_t;
	typedef		u64			xthread_tid_t;
	typedef		pthread_t	xthread_hnd_t;

	enum EThreadPriority
	{
		XTHREAD_PRIORITY_LOWEST = THREAD_PRIORITY_LOWEST,
		XTHREAD_PRIORITY_LOW = THREAD_PRIORITY_BELOW_NORMAL,
		XTHREAD_PRIORITY_NORMAL = THREAD_PRIORITY_NORMAL,
		XTHREAD_PRIORITY_HIGH = THREAD_PRIORITY_ABOVE_NORMAL,
		XTHREAD_PRIORITY_HIGHEST = THREAD_PRIORITY_HIGHEST
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
		xthread_impl();				
		~xthread_impl();

		xthread_idx_t	thread_get_idx() const;
		xthread_tid_t	thread_get_tid() const;

		void			thread_set_priority(EThreadPriority prio);
		EThreadPriority	thread_get_priority() const;

		void			thread_start();

		void			thread_join();
		bool			thread_join(u32 milliseconds);

		enum EThreadState
		{
			STATE_CREATED = 0,
			STATE_RUNNING = 1,
			STATE_STOPPED = 2
		};
		EThreadState	thread_get_state() const;

		static xthread_impl*	thread_current();
		static xthread_tid_t	thread_current_tid();

		static void		thread_create(xthread_impl* _this, xthread_config& _config, xthread_functor* _f);
		static void		thread_sleep(u32 milliseconds);
		static void		thread_yield();
		static void		thread_exit();

	protected:
		xthread_hnd_t		m_threadHandle;
		xthread_idx_t		m_threadIdx;
		xthread_tid_t		m_threadTid;
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
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}


	inline void xthread_impl::thread_yield()
	{
		::sleep(0);
	}


} // namespace ncore


#endif // __XMTHREAD_THREAD_OSX_H__
