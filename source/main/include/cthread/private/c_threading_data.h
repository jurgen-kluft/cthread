#ifndef __CTHREAD_THREADING_SYSTEM_H__
#define __CTHREAD_THREADING_SYSTEM_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_allocator_pool.h"

#include "cthread/c_thread.h"
#include "cthread/c_mutex.h"
#include "cthread/c_event.h"
#include "cthread/c_semaphore.h"

namespace ncore
{
    namespace nthread
    {
        bool mutex_data_init(mutex_data_t* data);
        void mutex_data_release(mutex_data_t* data);

        bool event_data_init(event_data_t* data, bool autoReset);
        void event_data_release(event_data_t* data);

        bool sema_data_init(sema_data_t* data, s32 n, s32 max);
        void sema_data_release(sema_data_t* data);

        struct system_t
        {
            static void setup(system_t* system, alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores);
            static void teardown(system_t* system);

            alloc_t*                    m_allocator;
            u32                         m_thread_priority_map[npriority::COUNT];
            fixed_pool_t<thread_t>      m_threads_pool;
            fixed_pool_t<thread_data_t> m_threads_data_pool;
            fixed_pool_t<event_t>       m_events_pool;
            fixed_pool_t<event_data_t>  m_events_data_pool;
            fixed_pool_t<mutex_t>       m_mutexes_pool;
            fixed_pool_t<mutex_data_t>  m_mutexes_data_pool;
            fixed_pool_t<sema_t>        m_semaphores_pool;
            fixed_pool_t<sema_data_t>   m_semaphores_data_pool;
        };
    } // namespace nthread
} // namespace ncore

#endif
