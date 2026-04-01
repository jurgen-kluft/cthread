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
    namespace nthread
    {
        system_t* create_system(alloc_t* allocator, u32 max_threads, u32 max_mutex, u32 max_event, u32 max_semaphore)
        {
            void*     mem = g_allocate<system_t>(allocator);
            system_t* sys = new (mem) system_t();

            if (s_system == nullptr)
                s_system = sys;

            system_t::setup(sys, allocator, max_threads, max_mutex, max_event, max_semaphore);

            // Initialize data instance
            init_thread_priority(sys->m_thread_priority_map);

            return sys;
        }

        void destroy_system(system_t*& sys)
        {
            alloc_t* allocator = sys->m_allocator;
            system_t::teardown(sys);
            allocator->deallocate(sys);
            sys = nullptr;
        }

        system_t* s_system = nullptr;

        mutex_t* create_mutex()
        {
            mutex_data_t* data = s_system->m_mutexes_data_pool.allocate();
            if (data)
            {
                mutex_t* m = s_system->m_mutexes_pool.allocate();
                if (m)
                {
                    mutex_data_init(data);
                    return m;
                }
            }
            return nullptr;
        }

        event_t* create_event(const char* name, bool autoReset)
        {
            system_t*     sys  = s_system;
            event_data_t* data = sys->m_events_data_pool.allocate();
            if (data)
            {
                event_t* e = sys->m_events_pool.allocate();
                if (e)
                {
                    event_data_init(data, autoReset);
                    return e;
                }
                sys->m_events_data_pool.deallocate(data);
            }
            return nullptr;
        }

        sema_t* create_sema(s32 initial_count, s32 max_count)
        {
            system_t*    sys  = s_system;
            sema_data_t* data = sys->m_semaphores_data_pool.allocate();
            if (data)
            {
                sema_t* s = sys->m_semaphores_pool.allocate();
                if (s)
                {
                    sema_data_init(data, initial_count, max_count);
                    return s;
                }
                sys->m_semaphores_data_pool.deallocate(data);
            }
            return nullptr;
        }

        void destroy(thread_t* t)
        {
            system_t* sys = s_system;
            sys->m_threads_data_pool.deallocate(t->m_data);
            sys->m_threads_pool.deallocate(t);
        }
        void destroy(mutex_t* m)
        {
            system_t* sys = s_system;
            sys->m_mutexes_data_pool.deallocate(m->m_data);
            sys->m_mutexes_pool.deallocate(m);
        }
        void destroy(event_t* e)
        {
            system_t* sys = s_system;
            sys->m_events_data_pool.deallocate(e->m_data);
            sys->m_events_pool.deallocate(e);
        }
        void destroy(sema_t* s)
        {
            system_t* sys = s_system;
            sys->m_semaphores_data_pool.deallocate(s->m_data);
            sys->m_semaphores_pool.deallocate(s);
        }
    } // namespace nthread
} // namespace ncore
