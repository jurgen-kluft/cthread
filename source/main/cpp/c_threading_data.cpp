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
        threading_data_t::threading_data_t()
            : m_allocator(nullptr)
            , m_threads_pool()
            , m_threads_data_pool()
            , m_events_pool()
            , m_events_data_pool()
            , m_mutexes_pool()
            , m_mutexes_data_pool()
            , m_semaphores_pool()
            , m_semaphores_data_pool()
        {
        }

        threading_data_t* threading_data_t::create(alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores)
        {
            threading_data_t* data = g_construct<threading_data_t>(allocator);

            data->m_allocator = allocator;
            data->m_threads_pool.setup(allocator, max_threads);
            data->m_threads_pool.setup(allocator, max_threads);
            data->m_mutexes_pool.setup(allocator, max_mutexes);
            data->m_events_pool.setup(allocator, max_events);
            data->m_semaphores_pool.setup(allocator, max_semaphores);

            data->m_threads_data_pool.setup(allocator, max_threads);
            data->m_threads_data_pool.setup(allocator, max_threads);
            data->m_mutexes_data_pool.setup(allocator, max_mutexes);
            data->m_events_data_pool.setup(allocator, max_events);
            data->m_semaphores_data_pool.setup(allocator, max_semaphores);

            return data;
        }

        void threading_data_t::destroy(threading_data_t*& data)
        {
            alloc_t* allocator = data->m_allocator;

            data->m_threads_pool.teardown(allocator);
            data->m_threads_pool.teardown(allocator);
            data->m_mutexes_pool.teardown(allocator);
            data->m_events_pool.teardown(allocator);
            data->m_semaphores_pool.teardown(allocator);

            data->m_threads_data_pool.teardown(allocator);
            data->m_threads_data_pool.teardown(allocator);
            data->m_mutexes_data_pool.teardown(allocator);
            data->m_events_data_pool.teardown(allocator);
            data->m_semaphores_data_pool.teardown(allocator);

            allocator->deallocate(data);
            data = nullptr;
        }
    } // namespace nthread
} // namespace ncore
