#include "ccore/c_target.h"

#if defined(TARGET_MAC) && !defined(TARGET_TEST)

#    include "cthread/c_thread.h"
#    include "cthread/private/c_thread_mac.h"
#    include "cthread/private/c_threading_data.h"

#    include <pthread.h>
#    include <semaphore.h>
#    include <sys/time.h>

namespace ncore
{
    namespace nthread
    {
        // Do we really need this, in cbase we already have thread_context_t which is bound to thread local.
        thread_local thread_data_t* t_current_thread_data = nullptr;

        thread_data_t* thread_data_t::current() { return t_current_thread_data; }
        thread_t*      current_thread() { return t_current_thread_data->m_thread; }

        void* __thread_main(void* arg)
        {
            thread_t*      t = reinterpret_cast<thread_t*>(arg);
            thread_data_t* d = t->m_data;

            // Set the thread ID now that we are in the thread
            d->m_tid = (int)reinterpret_cast<long long>(pthread_self());

            t_current_thread_data = d; // Set the current thread data as thread local
            {
                d->m_start_fn(t);
                d->m_state = nstate::RUNNING;
                d->m_run_fn(t);
                d->m_state = nstate::STOPPED;
                d->m_exit_fn(t);
            }
            t_current_thread_data = nullptr; // Clear the thread local data

            // Destroy the thread
            pthread_detach(d->m_pthread);
            d->m_pthread = 0;

            // Remove 'thread_t' and 'thread_data_t' from the sys system
            if (s_system)
            {
                destroy(t);
            }

            return 0;
        }

        s32 thread_construct(thread_t* t)
        {
            s32 error = 0;
            if (t->m_data->m_pthread == 0)
            {
                pthread_attr_t attr;
                if (pthread_attr_init(&attr))
                {
                    error = -1; // Could not initialize pthread attribute structure
                    return error;
                }

                if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
                {
                    // Could not set detach state in thread attribute structure
                    error = -2;
                    return error;
                }

                if (pthread_create(&t->m_data->m_pthread, &attr, __thread_main, reinterpret_cast<void*>(t)))
                {
                    //  pthread_create failed
                    error = -3;
                    return error;
                }
            }

            return error;
        }

        void thread_set_priority(thread_t* t, priority_t priority)
        {
            if (priority != t->m_data->m_priority)
            {
                t->m_data->m_priority = priority;
                if (t->m_data->m_thread)
                {
                    system_t* sys  = s_system;
                    u32 const prio = sys->m_thread_priority_map[t->m_data->m_priority];

                    sched_param sp;
                    int         sched_policy = 0;
                    if (pthread_getschedparam(t->m_data->m_pthread, &sched_policy, &sp) == 0)
                    {
                        s32 minPriority     = sched_get_priority_min(sched_policy);
                        s32 maxPriority     = sched_get_priority_max(sched_policy);
                        s32 priorityQuantum = (maxPriority - minPriority) / 6;
                        s32 normalPriority  = (maxPriority - minPriority) / 2;

                        sp.sched_priority = normalPriority;
                        if (t->m_data->m_priority == npriority::HIGH || t->m_data->m_priority == npriority::ABOVE_NORMAL)
                        {
                            sp.sched_priority = normalPriority + priorityQuantum;
                        }
                        else if (t->m_data->m_priority == npriority::IDLE)
                        {
                            sp.sched_priority = minPriority;
                        }
                        else if (t->m_data->m_priority == npriority::BELOW_NORMAL || t->m_data->m_priority == npriority::LOW)
                        {
                            sp.sched_priority = normalPriority - priorityQuantum;
                        }
                        else if (t->m_data->m_priority == npriority::CRITICAL)
                        {
                            sp.sched_priority = maxPriority;
                        }

                        pthread_setschedparam(t->m_data->m_pthread, sched_policy, &sp);
                    }
                }
            }
        }

        void thread_start(thread_t* t)
        {
            if (t->m_data->m_state != nstate::RUNNING)
            {
                thread_construct(t);
            }
        }

        s32 thread_join(thread_t* t)
        {
            if (!t->m_data->m_thread || t->m_data->m_state == nstate::STOPPED)
                return 0;

            if (pthread_self() == t->m_data->m_pthread)
            {
                // Thread waiting on itself to finish?
                return -1;
            }

            void* returnValue = 0L;
            int   r           = pthread_join(t->m_data->m_pthread, &returnValue);
            if (r != 0)
            {
                // pthread_join failed!
                return -1;
            }
            t->m_data->m_state = nstate::STOPPED;
            return 0;
        }
    } // namespace nthread
} // namespace ncore

#endif
