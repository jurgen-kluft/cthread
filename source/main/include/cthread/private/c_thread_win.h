#ifndef __CTHREAD_THREAD_WIN_H__
#define __CTHREAD_THREAD_WIN_H__
#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/c_types.h"

#include <Windows.h>

namespace ncore
{
    namespace nthread
    {
        typedef void* hnd_t;

        struct thread_data_t
        {
            hnd_t      m_handle;
            index_t    m_idx;
            id_t       m_tid;
            state_t    m_state;
            priority_t m_priority;
            u32        m_stack_size;
            start_fn   m_start_fn;
            run_fn     m_run_fn;
            exit_fn    m_exit_fn;
            quit_fn    m_quit_fn;
            void*      m_arg;
            char       m_name[64];
        };

        struct event_data_t
        {
            void* m_event;
        };

        struct mutex_data_t
        {
            CRITICAL_SECTION _cs;
        };

        struct sema_data_t
        {
            void* _sema;
        };
    } // namespace nthread
} // namespace ncore

#endif // __CTHREAD_THREAD_WIN_H__
