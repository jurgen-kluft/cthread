#ifndef __CMTHREAD_THREADING_H__
#define __CMTHREAD_THREADING_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

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
    struct events_data_t;
    struct mutexes_data_t;
    struct semaphores_data_t;

    class threading_t
    {
    public:
        static threading_t* create(alloc_t* allocator, u32 max_threads = 32, u32 max_mutex = 32, u32 max_event = 32, u32 max_semaphore = 32);
        static void         destroy(threading_t*&);

        static void         set_instance(threading_t* instance);
        static threading_t* instance();

        thread_t* create_thread(const char* name, void* arg, thread_functor* f, u32 stack_size, thread_priority_t priority);
        mutex_t*  create_mutex();
        event_t*  create_event(bool autoReset);
        sema_t*   create_sema(s32 initial_count, s32 max_count);

        void destroy_thread(thread_t*);
        void destroy_mutex(mutex_t*);
        void destroy_event(event_t*);
        void destroy_sema(sema_t*);

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
        ~threading_t();

        threading_t(const threading_t&) {}
        threading_t& operator=(const threading_t&) { return *this; }

        static threading_t* instance_;

        friend class alloc_t;
        alloc_t*       m_allocator;
        thread_data_t* m_threads;
        event_data_t*  m_events;
        mutex_data_t*  m_mutexes;
        sema_data_t*   m_semaphores;
    };

} // namespace ncore

#endif