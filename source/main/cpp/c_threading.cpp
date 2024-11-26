#include "ccore/c_target.h"

#include "cbase/c_allocator.h"

#include "cthread/c_types.h"
#include "cthread/c_thread.h"
#include "cthread/c_mutex.h"
#include "cthread/c_event.h"
#include "cthread/c_semaphore.h"

#if defined(TARGET_MAC) && !defined(TARGET_TEST)
#    include "cthread/private/c_thread_mac.h"
#endif
#if defined(TARGET_PC) && !defined(TARGET_TEST)
#    include "cthread/private/c_thread_win.h"
#endif
#if defined(TARGET_TEST)
#    include "cthread/private/c_thread_mock.h"
#endif

#include "cthread/c_threading.h"
#include "cthread/private/c_threading_data.h"

namespace ncore
{
    threading_t* threading_t::create(alloc_t* allocator, u32 max_threads, u32 max_mutex, u32 max_event, u32 max_semaphore)
    {
        threading_t* threading = (threading_t*)allocator->allocate(sizeof(threading_t));
        threading->m_data      = nullptr;

        if (instance() == threading)
            set_instance(nullptr);

        threading->m_data = threading_data_t::create(allocator, max_threads, max_mutex, max_event, max_semaphore);

        // Initialize data instance
        init_thread_priority(threading->m_data->m_thread_priority_map);

        return threading;
    }

    void threading_t::destroy(threading_t*& threading)
    {
        threading->~threading_t();
        threading->m_data->m_allocator->deallocate(threading);
        threading->m_data = nullptr;
        threading_data_t::destroy(threading->m_data);
    }

    static threading_t* s_instance = nullptr;
    void                threading_t::set_instance(threading_t* instance) { s_instance = instance; }
    threading_t*        threading_t::instance() { return s_instance; }

    mutex_t* threading_t::create_mutex()
    {
        mutex_data_t* data = m_data->m_mutexes_data_pool.allocate();
        if (data)
        {
            mutex_t* m = m_data->m_mutexes_pool.allocate();
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
        event_data_t* data = m_data->m_events_data_pool.allocate();
        if (data)
        {
            event_t* e = m_data->m_events_pool.allocate();
            if (e)
            {
                e->init(data, autoReset);
                return e;
            }
            m_data->m_events_data_pool.deallocate(data);
        }
        return nullptr;
    }

    sema_t* threading_t::create_sema(s32 initial_count, s32 max_count)
    {
        sema_data_t* data = m_data->m_semaphores_data_pool.allocate();
        if (data)
        {
            sema_t* s = m_data->m_semaphores_pool.allocate();
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
        m_data->m_threads_data_pool.deallocate(t->m_data);
        m_data->m_threads_pool.deallocate(t);
    }
    void threading_t::destroy(mutex_t* m)
    {
        m_data->m_mutexes_data_pool.deallocate(m->m_data);
        m_data->m_mutexes_pool.deallocate(m);
    }
    void threading_t::destroy(event_t* e)
    {
        m_data->m_events_data_pool.deallocate(e->m_data);
        m_data->m_events_pool.deallocate(e);
    }
    void threading_t::destroy(sema_t* s)
    {
        m_data->m_semaphores_data_pool.deallocate(s->m_data);
        m_data->m_semaphores_pool.deallocate(s);
    }

} // namespace ncore
