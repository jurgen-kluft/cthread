#ifndef __CMTHREAD_THREAD_H__
#define __CMTHREAD_THREAD_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cthread/c_types.h"

namespace ncore
{
    class thread_functor;

    class thread_t
    {
    public:
        enum e_config
        {
            DEFAULT_STACKSIZE = 256 * 1024,
            DEFAULT_PRIORITY  = thread_priority_t::NORMAL
        };

        enum e_state
        {
            STATE_CREATED = 0,
            STATE_RUNNING = 1,
            STATE_STOPPED = 2
        };

        thread_t();

        thread_id_t       get_tid() const;                   // Returns the native thread ID of the thread.
        thread_idx_t      get_idx() const;                   // Returns the unique thread index of the thread.
        const char*       get_name() const;                  // Returns the name of the thread.
        thread_priority_t get_priority() const;              // Returns the thread's priority.
        e_state           get_state() const;                 // Returns state of the thread
        bool              is_running() const;                // Returns true if the thread is running.
        u32               get_stacksize() const;             // Returns the stack size of the thread.
        void              set_priority(thread_priority_t p); // Sets the thread's priority.

        void create(const char* name, thread_functor* functor, u32 stacksize = 0, thread_priority_t priority = thread_priority_t::NORMAL);
        void start();
        void join();
        bool join(u32 milliseconds);

        static u32               default_stacksize();
        static thread_priority_t default_priority();

    protected:
        friend class nthread;

        thread_t(const thread_t&);
        thread_t& operator=(const thread_t&);
        ~thread_t(); /// Destroys the thread.

        e_state           m_state;
        thread_id_t       m_tid;
        thread_idx_t      m_idx;
        char*             m_name;
        thread_priority_t m_priority;
        u32               m_stacksize;
    };

} // namespace ncore

#endif // __CMTHREAD_THREAD_H__
