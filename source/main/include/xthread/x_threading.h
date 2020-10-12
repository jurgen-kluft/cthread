#ifndef __XMTHREAD_THREADING_H__
#define __XMTHREAD_THREADING_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_allocator.h"
#include "xthread/x_thread.h"

namespace xcore
{
    class xalloc;

	class xthread_functor
	{
	public:	
		virtual			~xthread_functor() {}
		virtual void	run() = 0;
	};

    class xthread;
    class xmutex;
    class xevent;
    class xsemaphore;

    class xthreads_data;
    class xevents_data;
    class xmutexes_data;
	class xsemaphores_data;

	class xthreading
	{
	public:
		static xthreading*	create(xalloc* allocator, u32 max_threads = 32, u32 max_mutex = 32, u32 max_event = 32, u32 max_semaphore = 32);

		xthread*            create_thread(const char* name, void* arg, xthread_functor* f, u32 stack_size, xthread::e_priority priority);
		xmutex*				create_mutex();
		xevent*				create_event(bool autoReset);
		xsemaphore*			create_semaphore(s32 initial_count, s32 max_count);

		void				destroy_thread(xthread*);
		void				destroy_mutex(xmutex*);
		void				destroy_event(xevent*);
		void				destroy_semaphore(xsemaphore*);

		void				join(xthread*);
		bool				join(xthread*, u32 milliseconds);

		static xthread*		current();
		static void			sleep(u32 milliseconds);
		static void			yield();
		static void			exit();

		XCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
							xthreading();
							~xthreading();

							xthreading(const xthreading&) {}
		xthreading&			operator = (const xthreading&) { return *this; }

		xthreads_data*      m_threads;
		xevents_data*       m_events;
		xmutexes_data*      m_mutexes;
		xsemaphores_data*   m_semaphores;
	};

} // namespace xcore


#endif 