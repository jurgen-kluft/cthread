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


    class threading_t
    {
    public:
        static threading_t* create(alloc_t* allocator,  u32 max_threads = 32, u32 max_mutex = 256, u32 max_event = 256, u32 max_semaphore = 256);
        static void         destroy(threading_t*&);

        static void         set_instance(threading_t* instance);
        static threading_t* instance();

        thread_t* create_thread(const char* name, thread_functor* f, u32 stack_size, thread_priority_t priority);
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

        static void         sleep(u32 milliseconds);
        static void         yield();
        static void         exit();

        DCORE_CLASS_PLACEMENT_NEW_DELETE

    protected:
        friend class thread_t;

        // Platform specifics
        static void init_thread_priority(u32* map);

        template <typename T> struct data_t
        {
        };

        static threading_t* s_instance;
        alloc_t*            m_allocator;

        u32              m_thread_priority_map[thread_priority_t::COUNT];
        thread_t*        m_threads;
        fsadexed_array_t m_threads_pool;
        thread_data_t*   m_threads_data;
        fsadexed_array_t m_threads_data_pool;
        event_t*         m_events;
        fsadexed_array_t m_events_pool;
        event_data_t*    m_events_data;
        fsadexed_array_t m_events_data_pool;
        mutex_t*         m_mutexes;
        fsadexed_array_t m_mutexes_pool;
        mutex_data_t*    m_mutexes_data;
        fsadexed_array_t m_mutexes_data_pool;
        sema_t*          m_semaphores;
        fsadexed_array_t m_semaphores_pool;
        sema_data_t*     m_semaphores_data;
        fsadexed_array_t m_semaphores_data_pool;

    private:
        threading_t();
        threading_t(const threading_t&) {}
        ~threading_t() {}
        threading_t& operator=(const threading_t&) { return *this; }
    };

} // namespace ncore

#endif
