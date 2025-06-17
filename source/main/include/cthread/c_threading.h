#ifndef __CMTHREAD_THREADING_H__
#define __CMTHREAD_THREADING_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"
#include "cthread/c_types.h"

namespace ncore
{
    class alloc_t;

    class thread_t;
    class mutex_t;
    class event_t;
    class sema_t;

    struct thread_data_t;
    struct event_data_t;
    struct mutex_data_t;
    struct sema_data_t;

    class thread_fn_t
    {
    public:
        virtual ~thread_fn_t() {}

        virtual void start(thread_t* t, thread_data_t* d) = 0;
        virtual void run()                                = 0;
        virtual void exit()                               = 0;
    };

    struct threading_data_t;
    class threading_t
    {
    public:
        static threading_t* create(alloc_t* allocator, u32 max_threads = 32, u32 max_mutex = 256, u32 max_event = 256, u32 max_semaphore = 256);
        static void         destroy(threading_t*&);

        static void         set_instance(threading_t* instance);
        static threading_t* instance();

        thread_t* create_thread(const char* name, thread_fn_t* f, thread_priority_t priority = thread_priority_t::NORMAL, u32 stack_size = 0);
        mutex_t*  create_mutex();
        event_t*  create_event(const char* name, bool autoReset);
        sema_t*   create_sema(s32 initial_count, s32 max_count);

        void destroy(thread_t*);
        void destroy(mutex_t*);
        void destroy(event_t*);
        void destroy(sema_t*);

        void join(thread_t*);

        static thread_t* current();

        static void sleep(u32 milliseconds);
        static void yield();
        static void exit();

        DCORE_CLASS_PLACEMENT_NEW_DELETE

    protected:
        friend class thread_t;

        // Platform specifics
        static void init_thread_priority(u32* map);

        threading_data_t* m_data;

        threading_t() : m_data(nullptr) {}
        threading_t(const threading_t&) = delete;
        ~threading_t() {}
        threading_t& operator=(const threading_t&) { return *this; }
    };

} // namespace ncore

#endif
