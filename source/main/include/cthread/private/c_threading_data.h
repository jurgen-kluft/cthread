#ifndef __CTHREAD_THREADING_DATA_H__
#define __CTHREAD_THREADING_DATA_H__
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
        template <typename T> class objects_array_t
        {
        public:
            fixed_pool_t<T> m_pool;
            inline T*       allocate() { return m_pool.allocate(); }
            inline void     deallocate(T* object) { m_pool.deallocate(object); }
            inline u32      ptr2idx(T const* object) const { return m_pool.obj2idx(object); }
            inline T*       idx2ptr(u32 idx) const { return m_pool.idx2obj(idx); }
            inline void     setup(alloc_t* allocator, u32 capacity) { m_pool.setup(allocator, capacity); }
            inline void     teardown(alloc_t* allocator) { m_pool.teardown(allocator); }
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
    } // namespace nthread
} // namespace ncore

#endif
