#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/private/c_threading.h"

namespace ncore
{
    extern threads_data_t*    gCreateThreadsData(alloc_t*, u32 max_threads);
    extern mutexes_data_t*    gCreateMutexesData(alloc_t*, u32 max_mutexes);
    extern events_data_t*     gCreateEventsData(alloc_t*, u32 max_events);
    extern semaphores_data_t* gCreateSemaphoresData(alloc_t*, u32 max_semaphores);

    threading_t* threading_t::create(alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores)
    {
        void*        threading_mem = allocator->allocate(sizeof(threading_t));
        threading_t* threading     = (threading_t*)(threading_mem);

        threading->m_allocator  = allocator;
        threading->m_threads    = gCreateThreadsData(allocator, max_threads);
        threading->m_mutexes    = gCreateMutexesData(allocator, max_mutexes);
        threading->m_events     = gCreateEventsData(allocator, max_events);
        threading->m_semaphores = gCreateSemaphoresData(allocator, max_semaphores);
        return threading;
    }

    void threading_t::destroy(threading_t*& threading_t)
    {
        threading_t->m_allocator->destruct(threading_t);
        threading_t = nullptr;
    }

} // namespace ncore
