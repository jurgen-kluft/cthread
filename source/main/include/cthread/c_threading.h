#ifndef __CMTHREAD_THREADING_H__
#define __CMTHREAD_THREADING_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_allocator.h"
#include "cbase/c_hbb.h"
#include "cthread/c_types.h"

namespace ncore
{
    class alloc_t;

    class thread_functor
    {
    public:
        virtual ~thread_functor() {}
        virtual void run(thread_t*) = 0;
    };

    class thread_t;
    class mutex_t;
    class event_t;
    class sema_t;

    struct thread_data_t;
    struct event_data_t;
    struct mutex_data_t;
    struct sema_data_t;

    class threading_t
    {
    public:
        static threading_t* create(alloc_t* allocator, u32 max_threads = 32, u32 max_mutex = 256, u32 max_event = 256, u32 max_semaphore = 256);
        static void         destroy(threading_t*&);

        static void         set_instance(threading_t* instance);
        static threading_t* instance();

        thread_t* create_thread(const char* name, void* arg, thread_functor* f, u32 stack_size, thread_priority_t priority);
        mutex_t*  create_mutex();
        event_t*  create_event(const char* name, bool autoReset);
        sema_t*   create_sema(s32 initial_count, s32 max_count);

        void destroy(thread_t*);
        void destroy(mutex_t*);
        void destroy(event_t*);
        void destroy(sema_t*);

        void join(thread_t*);
        bool join(thread_t*, u32 milliseconds);

        static thread_t*    current();
        static thread_id_t  current_tid();
        static thread_idx_t current_idx();
        static void         sleep(u32 milliseconds);
        static void         yield();
        static void         exit();

    private:
        threading_t();
        threading_t(const threading_t&) {}
        ~threading_t() {}
        threading_t& operator=(const threading_t&) { return *this; }

        template <typename T> struct data_t
        {
            T*               m_data;
            fsadexed_array_t m_pool;
        };

        static threading_t*   s_instance;
        alloc_t*              m_allocator;
        data_t<thread_data_t> m_threads;
        data_t<event_data_t>  m_events;
        data_t<mutex_data_t>  m_mutexes;
        data_t<sema_data_t>   m_semaphores;
    };

} // namespace ncore

#endif