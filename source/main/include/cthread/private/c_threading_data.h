#ifndef __CMTHREAD_THREADING_DATA_H__
#define __CMTHREAD_THREADING_DATA_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"

#include "cthread/c_thread.h"
#include "cthread/c_mutex.h"
#include "cthread/c_event.h"
#include "cthread/c_semaphore.h"

namespace ncore
{
    template <typename T> class objects_array_t
    {
    public:
        pool_t<T> m_pool;

        T* allocate() { return m_pool.allocate(); }
        void deallocate(T* object) { m_pool.deallocate(object); }
        T*   idx2ptr(u32 index) const { return m_pool.idx2ptr(index); }
        u32  ptr2idx(T const* ptr) const { return m_pool.ptr2idx(ptr); }

        void setup(alloc_t* allocator, u32 capacity)
        {
            T* data = g_allocate_array_and_memset<T>(allocator, capacity);
            m_pool.setup(data, capacity);
        }

        void teardown(alloc_t* allocator)
        {
            allocator->deallocate(m_pool.m_data);
            m_pool.m_data = nullptr;
        }
    };

    struct threading_data_t
    {
        threading_data_t();

        static threading_data_t* create(alloc_t* allocator, u32 max_threads, u32 max_mutexes, u32 max_events, u32 max_semaphores);
        static void              destroy(threading_data_t*& data);

        alloc_t*                       m_allocator;
        u32                            m_thread_priority_map[thread_priority_t::COUNT];
        objects_array_t<thread_t>      m_threads_pool;
        objects_array_t<thread_data_t> m_threads_data_pool;
        objects_array_t<event_t>       m_events_pool;
        objects_array_t<event_data_t>  m_events_data_pool;
        objects_array_t<mutex_t>       m_mutexes_pool;
        objects_array_t<mutex_data_t>  m_mutexes_data_pool;
        objects_array_t<sema_t>        m_semaphores_pool;
        objects_array_t<sema_data_t>   m_semaphores_data_pool;

        DCORE_CLASS_PLACEMENT_NEW_DELETE
    };
} // namespace ncore

#endif