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
        void system_t::setup(system_t* system, alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores)
        {
            system->m_allocator = allocator;
            system->m_threads_pool.setup(allocator, max_threads);
            system->m_threads_pool.setup(allocator, max_threads);
            system->m_mutexes_pool.setup(allocator, max_mutexes);
            system->m_events_pool.setup(allocator, max_events);
            system->m_semaphores_pool.setup(allocator, max_semaphores);

            system->m_threads_data_pool.setup(allocator, max_threads);
            system->m_threads_data_pool.setup(allocator, max_threads);
            system->m_mutexes_data_pool.setup(allocator, max_mutexes);
            system->m_events_data_pool.setup(allocator, max_events);
            system->m_semaphores_data_pool.setup(allocator, max_semaphores);
        }

        void system_t::teardown(system_t* system)
        {
            alloc_t* allocator = system->m_allocator;

            system->m_threads_pool.teardown(allocator);
            system->m_threads_pool.teardown(allocator);
            system->m_mutexes_pool.teardown(allocator);
            system->m_events_pool.teardown(allocator);
            system->m_semaphores_pool.teardown(allocator);

            system->m_threads_data_pool.teardown(allocator);
            system->m_threads_data_pool.teardown(allocator);
            system->m_mutexes_data_pool.teardown(allocator);
            system->m_events_data_pool.teardown(allocator);
            system->m_semaphores_data_pool.teardown(allocator);
        }
    } // namespace nthread
} // namespace ncore
