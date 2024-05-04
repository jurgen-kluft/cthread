#include "ccore/c_target.h"

#include "cthread/c_threading.h"

#include "cthread/c_types.h"
#include "cthread/c_thread.h"
#include "cthread/c_mutex.h"
#include "cthread/c_event.h"
#include "cthread/c_semaphore.h"

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
        nmem::memclr(data, sizeof(T) * capacity);
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
        void* threading_mem = allocator->allocate(sizeof(threading_t), alignof(threading_t));
        nmem::memclr(threading_mem, sizeof(threading_t));
        threading_t* threading = (threading_t*)(threading_mem);

        // Initialize threading instance
        init_thread_priority(threading->m_thread_priority_map);

        threading->m_allocator = allocator;
        gInitData(threading->m_threads, threading->m_threads_pool, allocator, max_threads);
        gInitData(threading->m_mutexes, threading->m_mutexes_pool, allocator, max_mutexes);
        gInitData(threading->m_events, threading->m_events_pool, allocator, max_events);
        gInitData(threading->m_semaphores, threading->m_semaphores_pool, allocator, max_semaphores);

        gInitData(threading->m_threads_data, threading->m_threads_data_pool, allocator, max_threads);
        gInitData(threading->m_mutexes_data, threading->m_mutexes_data_pool, allocator, max_mutexes);
        gInitData(threading->m_events_data, threading->m_events_data_pool, allocator, max_events);
        gInitData(threading->m_semaphores_data, threading->m_semaphores_data_pool, allocator, max_semaphores);

        return threading;
    }

    void threading_t::destroy(threading_t*& threading)
    {
        alloc_t* allocator = threading->m_allocator;
        gDestroyData(threading->m_threads, threading->m_threads_pool, allocator);
        gDestroyData(threading->m_mutexes, threading->m_mutexes_pool, allocator);
        gDestroyData(threading->m_events, threading->m_events_pool, allocator);
        gDestroyData(threading->m_semaphores, threading->m_semaphores_pool, allocator);

        gDestroyData(threading->m_threads_data, threading->m_threads_data_pool, allocator);
        gDestroyData(threading->m_mutexes_data, threading->m_mutexes_data_pool, allocator);
        gDestroyData(threading->m_events_data, threading->m_events_data_pool, allocator);
        gDestroyData(threading->m_semaphores_data, threading->m_semaphores_data_pool, allocator);

        if (instance() == threading)
            set_instance(nullptr);

        allocator->deallocate(threading);
        threading = nullptr;
    }

    threading_t* threading_t::s_instance = nullptr;
    void         threading_t::set_instance(threading_t* instance) { s_instance = instance; }
    threading_t* threading_t::instance() { return s_instance; }

    mutex_t* threading_t::create_mutex()
    {
        mutex_data_t* data = (mutex_data_t*)m_mutexes_data_pool.allocate();
        if (data)
        {
            mutex_t* m = (mutex_t*)m_mutexes_pool.allocate();
            if (m)
            {
                m->init(data);
                return m;
            }
        }
        return nullptr;
    }

    event_t* threading_t::create_event(const char* name, bool autoReset)
    {
        event_data_t* data = (event_data_t*)m_events_data_pool.allocate();
        if (data)
        {
            event_t* e = (event_t*)m_events_pool.allocate();
            if (e)
            {
                e->init(data, autoReset);
                return e;
            }
            m_events_data_pool.deallocate(data);
        }
        return nullptr;
    }

    sema_t* threading_t::create_sema(s32 initial_count, s32 max_count)
    {
        sema_data_t* data = (sema_data_t*)m_semaphores_data_pool.allocate();
        if (data)
        {
            sema_t* s = (sema_t*)m_semaphores_pool.allocate();
            if (s)
            {
                s->init(data, initial_count, max_count);
                return s;
            }
        }
        return nullptr;
    }

    void threading_t::destroy(thread_t* t)
    {
        m_threads_data_pool.deallocate(t->m_data);
        m_threads_pool.deallocate(t);
    }
    void threading_t::destroy(mutex_t* m)
    {
        m_mutexes_data_pool.deallocate(m->m_data);
        m_mutexes_pool.deallocate(m);
    }
    void threading_t::destroy(event_t* e)
    {
        m_events_data_pool.deallocate(e->m_data);
        m_events_pool.deallocate(e);
    }
    void threading_t::destroy(sema_t* s)
    {
        m_semaphores_data_pool.deallocate(s->m_data);
        m_semaphores_pool.deallocate(s);
    }

} // namespace ncore