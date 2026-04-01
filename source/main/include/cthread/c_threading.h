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

    namespace nthread
    {
        class thread_t;
        class mutex_t;
        class event_t;
        class sema_t;

        struct thread_data_t;
        struct event_data_t;
        struct mutex_data_t;
        struct sema_data_t;

        struct system_t;

        static system_t* create_system(alloc_t* allocator, u32 max_threads = 32, u32 max_mutex = 256, u32 max_event = 256, u32 max_semaphore = 256);
        static void      destroy_system(system_t*&);

        extern system_t* s_system;

        thread_t* create_thread(const char* name, start_fn start, run_fn run, exit_fn exit, quit_fn quit, priority_t priority = npriority::NORMAL, u32 stack_size = 0);
        mutex_t*  create_mutex();
        event_t*  create_event(const char* name, bool autoReset);
        sema_t*   create_sema(s32 initial_count, s32 max_count);

        void destroy(thread_t*);
        void destroy(mutex_t*);
        void destroy(event_t*);
        void destroy(sema_t*);
        void join(thread_t*);

        static thread_t* current(); // Returns the current thread

        static void sleep(u32 milliseconds); // Sleeps the current thread for the specified number of milliseconds
        static void yield();                 // Yields the current thread, allowing other threads to run
        static void quit();                  // Marks the current thread for quitting

        // Platform specifics
        static void init_thread_priority(u32* map);

    } // namespace nthread
} // namespace ncore

#endif
