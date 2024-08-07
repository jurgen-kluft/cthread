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
    struct thread_data_t;

    class thread_t
    {
    public:
        enum e_config
        {
            DEFAULT_STACKSIZE = 256 * 1024,
            DEFAULT_PRIORITY  = thread_priority_t::NORMAL
        };

        thread_id_t       get_tid() const;                   // Returns the native thread ID of the thread.
        thread_idx_t      get_idx() const;                   // Returns the unique thread index of the thread.
        const char*       get_name() const;                  // Returns the name of the thread.
        thread_priority_t get_priority() const;              // Returns the thread's priority.
        thread_state_t    get_state() const;                 // Returns state of the thread
        bool              is_running() const;                // Returns true if the thread is running.
        u32               get_stacksize() const;             // Returns the stack size of the thread.
        void              set_priority(thread_priority_t p); // Sets the thread's priority.

        void create(thread_data_t* data);
        void destroy();

        void start();
        void suspend();
        void resume();

        void join();
        bool join(u32 milliseconds);

        static u32               default_stacksize();
        static thread_priority_t default_priority();

    protected:
        friend class threading_t;
        thread_t()
            : m_data(nullptr)
        {
        }
        thread_data_t* m_data;

    private:
        thread_t(const thread_t&);
        thread_t& operator=(const thread_t&);
        ~thread_t(); /// Destroys the thread.
    };

} // namespace ncore

#endif // __CMTHREAD_THREAD_H__
