#ifndef __XMTHREAD_THREAD_WIN32_H__
#define __XMTHREAD_THREAD_WIN32_H__
#include "xbase/x_target.h"

namespace xcore
{
	class xthread;
	class xthread_functor;

#if defined(TARGET_DEVKIT)
	#define XTHREAD_DEBUGGER_THREAD_NAMES
#endif

	class ithread
	{
	public:
		typedef		s32			idx_t;
		typedef		u64			tid_t;
		typedef		void*		hnd_t;

		enum
		{
			DEFAULT_STACK_SIZE = 8192
		};

		class e_priority
		{
		public:
			inline		e_priority() : prio(NORMAL) {}
			inline		e_priority(s32 p) : prio(p) {}
			
			bool		operator == (const e_priority& p) const { return prio == p.prio; }
			bool		operator != (const e_priority& p) const { return prio != p.prio; }
			bool		operator <  (const e_priority& p) const { return prio <  p.prio; }
			bool		operator <= (const e_priority& p) const { return prio <= p.prio; }
			bool		operator >  (const e_priority& p) const { return prio >  p.prio; }
			bool		operator >= (const e_priority& p) const { return prio >= p.prio; }

			s32			prio;

			static const s32 LOWEST;
			static const s32 LOW;
			static const s32 NORMAL;
			static const s32 HIGH;
			static const s32 HIGHEST;
		};

		enum e_state
		{
			STATE_SETUP = 1,
			STATE_CREATED = 100,
			STATE_RUNNING = 200,
			STATE_STOPPED = 300
		};

		struct config
		{
			config()
				: m_priority(e_priority::NORMAL)
				, m_stack_size(DEFAULT_STACK_SIZE)
			{}

			e_priority	m_priority;
			u32			m_stack_size;
		};

		bool			is_running() const { return m_threadState == STATE_RUNNING; }

	protected:
						ithread(config const& _config);
						~ithread();

		idx_t			thread_get_idx() const;
		tid_t			thread_get_tid() const;

		void			thread_set_priority(e_priority prio);
		e_priority		thread_get_priority() const;

		u32				thread_get_stacksize() const;

		void			thread_start(xthread_functor* _f);
		void			thread_run();
		void			thread_exit();

		void			thread_join();
		bool			thread_join(u32 milliseconds);

		e_state			thread_get_state() const;

		static ithread*	thread_current();
		static tid_t	thread_current_tid();
		static idx_t	thread_current_idx();

		static void		thread_create(ithread* _this);
		static void		thread_sleep(u32 milliseconds);
		static void		thread_yield();

	protected:
		friend unsigned __stdcall 	__main_func(void* arg);

		hnd_t				m_threadHandle;
		idx_t				m_threadIdx;
		tid_t				m_threadTid;
		e_state				m_threadState;
		config				m_config;
		xthread_functor*	m_functor;
	};


	//
	// inlines
	//
	inline ithread::idx_t	ithread::thread_get_idx() const
	{
		return m_threadIdx;
	}

	inline ithread::tid_t ithread::thread_get_tid() const
	{
		return m_threadTid;
	}

	inline ithread::e_priority	ithread::thread_get_priority() const
	{
		return m_config.m_priority;
	}


} // namespace xcore


#endif // __XMTHREAD_THREAD_WIN32_H__
