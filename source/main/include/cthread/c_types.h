#ifndef __CTHREAD_TYPES_H__
#define __CTHREAD_TYPES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nthread
    {
        typedef s32 index_t;
        typedef u64 id_t;

        struct system_t;

        typedef void (*start_fn)(thread_t* t);
        typedef void (*run_fn)(thread_t* t);
        typedef void (*exit_fn)(thread_t* t);
        typedef void (*quit_fn)(thread_t* t);

        struct thread_data_t;
        struct thread_t
        {
            start_fn       m_start_fn;
            run_fn         m_run_fn;
            exit_fn        m_exit_fn;
            quit_fn        m_quit_fn;
            thread_data_t* m_data;
        };

        typedef s8 state_t;
        namespace nstate
        {
            enum
            {
                CREATED = 0,
                RUNNING = 1,
                STOPPED = 3
            };
        }; // namespace nstate

        typedef s8 priority_t;
        namespace npriority
        {
            enum
            {
                IDLE         = 0,
                LOW          = 1,
                BELOW_NORMAL = 2,
                NORMAL       = 3,
                ABOVE_NORMAL = 4,
                HIGH         = 5,
                CRITICAL     = 6,
                COUNT        = 7
            };
        }; // namespace npriority

    } // namespace nthread
} // namespace ncore

#endif // __CTHREAD_TYPES_H__
