#ifndef __CMTHREAD_THREAD_OSX_H__
#define __CMTHREAD_THREAD_OSX_H__
#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/c_types.h"

#include <thread>
#include <chrono>

#include <mach/mach.h>
#include <mach/semaphore.h>

namespace ncore
{
    typedef void* hnd_t;

    struct thread_data_t
    {
        hnd_t             m_handle;
        thread_idx_t      m_idx;
        thread_id_t       m_tid;
        thread_state_t    m_state;
        thread_priority_t m_priority;
        u32               m_stack_size;
        thread_functor*   m_functor;
        void*             m_arg;
        char              m_name[64];
    };

    struct event_data_t
    {
        std::atomic<s32> m_status;
        // Lightweight semaphore
        std::atomic<s32> m_count;
        ::semaphore_t    m_sema;
    };

    struct mutex_data_t
    {
        pthread_mutex_t _mutex;
    };

    struct sema_data_t
    {
        ::semaphore_t _sema;
    };

} // namespace ncore

#endif // __CMTHREAD_THREAD_OSX_H__
