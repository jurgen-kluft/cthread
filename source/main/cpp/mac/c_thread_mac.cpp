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
    thread_local thread_data_t* t_current_thread_data = nullptr;

    thread_data_t* thread_data_t::current() { return t_current_thread_data; }
    thread_t* threading_t::current() { return t_current_thread_data->m_thread; }

    void* __thread_main(void* arg)
    {
        thread_t*      t = reinterpret_cast<thread_t*>(arg);
        thread_data_t* d = t->get_data();

        // Set the thread ID now that we are in the thread
        d->m_tid = (int)reinterpret_cast<long long>(pthread_self());

        thread_fn_t* f = d->m_functor;
        {
            t_current_thread_data = d; // Set the current thread data as thread local
            {
                f->start(t, d);
                d->m_state = thread_state_t::RUNNING;
                f->run();
                d->m_state = thread_state_t::STOPPED;
                f->exit();
            }
            t_current_thread_data = nullptr; // Clear the thread local data
        }

        // Destroy the thread
        pthread_detach(d->m_pthread);
        d->m_pthread = 0;

        // Remove 'thread_t' and 'thread_data_t' from the threading system
        threading_t* threading = threading_t::instance();
        if (threading)
        {
            threading->destroy(t);
        }

        return 0;
    }

    s32 thread_t::create()
    {
        s32 error = 0;
        if (m_data->m_pthread == 0)
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

            if (pthread_create(&m_data->m_pthread, &attr, __thread_main, reinterpret_cast<void*>(this)))
            {
                //  pthread_create failed
                error = -3;
                return error;
            }
        }

        return error;
    }

    void thread_t::set_priority(thread_priority_t priority)
    {
        if (priority != m_data->m_priority)
        {
            m_data->m_priority = priority;
            if (m_data->m_thread)
            {
                threading_t* threading = threading_t::instance();
                u32 const    prio      = threading->m_data->m_thread_priority_map[m_data->m_priority.prio];

                sched_param sp;
                int         sched_policy = 0;
                if (pthread_getschedparam(m_data->m_pthread, &sched_policy, &sp) == 0)
                {
                    s32 minPriority     = sched_get_priority_min(sched_policy);
                    s32 maxPriority     = sched_get_priority_max(sched_policy);
                    s32 priorityQuantum = (maxPriority - minPriority) / 6;
                    s32 normalPriority  = (maxPriority - minPriority) / 2;

                    sp.sched_priority = normalPriority;
                    if (m_data->m_priority == thread_priority_t::HIGH || m_data->m_priority == thread_priority_t::ABOVE_NORMAL)
                    {
                        sp.sched_priority = normalPriority + priorityQuantum;
                    }
                    else if (m_data->m_priority == thread_priority_t::IDLE)
                    {
                        sp.sched_priority = minPriority;
                    }
                    else if (m_data->m_priority == thread_priority_t::BELOW_NORMAL || m_data->m_priority == thread_priority_t::LOW)
                    {
                        sp.sched_priority = normalPriority - priorityQuantum;
                    }
                    else if (m_data->m_priority == thread_priority_t::CRITICAL)
                    {
                        sp.sched_priority = maxPriority;
                    }

                    pthread_setschedparam(m_data->m_pthread, sched_policy, &sp);
                }
            }
        }
    }

    void thread_t::start()
    {
        if (m_data->m_state != thread_state_t::RUNNING)
        {
            create();
        }
    }

    s32 thread_t::join()
    {
        if (!m_data->m_thread || m_data->m_state == thread_state_t::STOPPED)
            return 0;

        if (pthread_self() == m_data->m_pthread)
        {
            // Thread waiting on itself to finish?
            return -1;
        }

        void* returnValue = 0L;
        int   r           = pthread_join(m_data->m_pthread, &returnValue);
        if (r != 0)
        {
            // pthread_join failed!
            return -1;
        }
        m_data->m_state = thread_state_t::STOPPED;
        return 0;
    }

} // namespace ncore

#endif
