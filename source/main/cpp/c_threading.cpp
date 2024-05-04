#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/private/c_threading.h"

#ifdef TARGET_MAC
#    include "cthread/private/c_thread_mac.h"
#endif

#ifdef TARGET_WIN
#    include "cthread/private/c_thread_win.h"
#endif

namespace ncore
{
    template <typename T> void gInitData(T*& data, fsadexed_array_t& fsa, alloc_t* allocator, u32 capacity)
    {
        data = (T*)allocator->allocate(sizeof(T) * capacity);
        for (u32 i = 0; i < capacity; ++i)
            data[i] = T();
        fsa = fsadexed_array_t(data, sizeof(T), capacity);
    }

    template <typename T> void gDestroyData(T*& data, fsadexed_array_t& fsa, alloc_t* allocator)
    {
        fsa = fsadexed_array_t();
        allocator->deallocate(data);
        data = nullptr;
    }

    threading_t* threading_t::create(alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores)
    {
        void*        threading_mem = allocator->allocate(sizeof(threading_t), alignof(threading_t));
        threading_t* threading     = (threading_t*)(threading_mem);

        threading->m_allocator = allocator;
        gInitData(threading->m_threads_data, threading->m_threads_pool, allocator, max_threads);
        gInitData(threading->m_mutexes_data, threading->m_mutexes_pool, allocator, max_mutexes);
        gInitData(threading->m_events_data, threading->m_events_pool, allocator, max_events);
        gInitData(threading->m_semaphores_data, threading->m_semaphores_pool, allocator, max_semaphores);
        return threading;
    }

    void threading_t::destroy(threading_t*& threading)
    {
        alloc_t* allocator = threading->m_allocator;
        gDestroyData(threading->m_threads_data, threading->m_threads_pool, allocator);
        gDestroyData(threading->m_mutexes_data, threading->m_mutexes_pool, allocator);
        gDestroyData(threading->m_events_data, threading->m_events_pool, allocator);
        gDestroyData(threading->m_semaphores_data, threading->m_semaphores_pool, allocator);

        if (instance() == threading)
            set_instance(nullptr);

        allocator->deallocate(threading);
        threading = nullptr;
    }

    void         threading_t::set_instance(threading_t* instance) { s_instance = instance; }
    threading_t* threading_t::instance() { return s_instance; }

} // namespace ncore