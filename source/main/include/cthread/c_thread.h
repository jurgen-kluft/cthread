#ifndef __CMTHREAD_THREAD_H__
#define __CMTHREAD_THREAD_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cthread/c_types.h"

namespace ncore
{
    namespace nthread
    {
        s32         thread_construct(thread_t* t);                  // return any negative value if failed
        id_t        thread_get_tid(thread_t* t);                    // Returns the native thread ID of the thread.
        index_t     thread_get_idx(thread_t* t);                    // Returns the unique thread index of the thread.
        const char* thread_get_name(thread_t* t);                   // Returns the name of the thread.
        priority_t  thread_get_priority(thread_t* t);               // Returns the thread's priority.
        state_t     thread_get_state(thread_t* t);                  // Returns state of the thread
        system_t*   thread_get_data(thread_t* t);                   // Returns the thread data.
        bool        thread_is_running(thread_t* t);                 // Returns true if the thread is running.
        u32         thread_get_stacksize(thread_t* t);              // Returns the stack size of the thread.
        void        thread_set_priority(thread_t* t, priority_t p); // Sets the thread's priority.
        void        thread_start(thread_t* t);                      // Starts the thread, returns 0 if the thread is started successfully, otherwise negative value.
        s32         thread_join(thread_t* t);                       // Returns 0 if the thread is joined successfully, otherwise negative value.
        u32         thread_default_stacksize();
        priority_t  thread_default_priority();

    } // namespace nthread
} // namespace ncore

#endif // __CMTHREAD_THREAD_H__
