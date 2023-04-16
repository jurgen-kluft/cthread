#ifndef __CMTHREAD_THREADING_H__
#define __CMTHREAD_THREADING_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_allocator.h"
#include "cthread/c_thread.h"

namespace ncore
{
    class alloc_t;

	class xthread_functor
	{
	public:	
		virtual			~xthread_functor() {}
		virtual void	run() = 0;
	};

    class cthread;
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
		static xthreading*	create(alloc_t* allocator, u32 max_threads = 32, u32 max_mutex = 32, u32 max_event = 32, u32 max_semaphore = 32);
		static void         destroy(xthreading*&);

		cthread*            create_thread(const char* name, void* arg, xthread_functor* f, u32 stack_size, cthread::e_priority priority);
		xmutex*				create_mutex();
		xevent*				create_event(bool autoReset);
		xsemaphore*			create_semaphore(s32 initial_count, s32 max_count);

		void				destroy_thread(cthread*);
		void				destroy_mutex(xmutex*);
		void				destroy_event(xevent*);
		void				destroy_semaphore(xsemaphore*);

		void				join(cthread*);
		bool				join(cthread*, u32 milliseconds);

		static cthread*		current();
		static void			sleep(u32 milliseconds);
		static void			yield();
		static void			exit();

		DCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
							xthreading();
							~xthreading();

							xthreading(const xthreading&) {}
		xthreading&			operator = (const xthreading&) { return *this; }

		friend class alloc_t;
		alloc_t*             m_allocator;
		xthreads_data*      m_threads;
		xevents_data*       m_events;
		xmutexes_data*      m_mutexes;
		xsemaphores_data*   m_semaphores;
	};

} // namespace ncore


#endif 