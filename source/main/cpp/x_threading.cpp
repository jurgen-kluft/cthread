#include "xbase/x_target.h"

#include "xthread/x_threading.h"
#include "xthread/private/x_threading.h"

namespace xcore 
{
	extern xthreads_data*		gCreateThreadsData(alloc_t*, u32 max_threads);
	extern xmutexes_data*		gCreateMutexesData(alloc_t*, u32 max_mutexes);
	extern xevents_data*		gCreateEventsData(alloc_t*, u32 max_events);
	extern xsemaphores_data*	gCreateSemaphoresData(alloc_t*, u32 max_semaphores);

	xthreading*	xthreading::create(alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores)
	{
		void* threading_mem = allocator->allocate(sizeof(xthreading));
		xthreading* threading = new (threading_mem) xthreading();

		threading->m_allocator = allocator;
		threading->m_threads = gCreateThreadsData(allocator, max_threads);
		threading->m_mutexes = gCreateMutexesData(allocator, max_mutexes);
		threading->m_events = gCreateEventsData(allocator, max_events);
		threading->m_semaphores = gCreateSemaphoresData(allocator, max_semaphores);
		return threading;
	}

	void xthreading::destroy(xthreading*& threading)
	{
		threading->m_allocator->destruct(threading);
		threading = nullptr;
	}

} // namespace xcore
