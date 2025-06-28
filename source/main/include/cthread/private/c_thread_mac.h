#ifndef __CMTHREAD_THREAD_MACOSX_H__
#define __CMTHREAD_THREAD_MACOSX_H__
#include "ccore/c_target.h"
#include "cbase/c_context.h"

#include "cthread/c_threading.h"
#include "cthread/c_types.h"

#include <thread>
#include <chrono>

#include <pthread.h>
#include <mach/mach.h>
#include <mach/semaphore.h>

namespace ncore
{
    namespace nthread
    {
        struct thread_data_t
        {
            pthread_t         m_pthread;
            thread_t*         m_thread;
            context_t         m_context;
            thread_idx_t      m_idx;
            thread_id_t       m_tid;
            thread_state_t    m_state;
            thread_priority_t m_priority;
            u32               m_stack_size; // TODO Is this necessary?
            thread_fn_t*      m_functor;
            void*             m_arg;
            char              m_name[64];

            static thread_data_t* current();
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
    } // namespace nthread
} // namespace ncore

#endif // __CMTHREAD_THREAD_MACOSX_H__
