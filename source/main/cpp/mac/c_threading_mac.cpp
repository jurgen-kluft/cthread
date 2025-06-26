#include "ccore/c_target.h"

#if defined(TARGET_MAC) && !defined(TARGET_TEST)
#    include "cthread/c_types.h"
#    include "cthread/c_thread.h"
#    include "cthread/c_mutex.h"
#    include "cthread/c_event.h"
#    include "cthread/c_semaphore.h"
#    include "cthread/private/c_thread_mac.h"
#    include "cthread/private/c_threading_data.h"

namespace ncore
{
    namespace nthread
    {
        thread_t* threading_t::create_thread(const char* name, thread_fn_t* f, thread_priority_t priority, u32 stack_size)
        {
            thread_data_t* data = (thread_data_t*)m_data->m_threads_data_pool.allocate();
            if (data)
            {
                thread_t* t = (thread_t*)m_data->m_threads_pool.allocate();
                if (t)
                {
                    t->m_data               = data;
                    t->m_data->m_thread     = t;
                    t->m_data->m_pthread    = 0;
                    t->m_data->m_functor    = f;
                    t->m_data->m_stack_size = stack_size;
                    t->m_data->m_priority   = priority;
                    t->m_data->m_state      = thread_state_t::CREATED;
                    t->m_data->m_idx        = m_data->m_threads_pool.ptr2idx(t);
                    t->m_data->m_tid        = 0;
                    t->m_data->m_name[0]    = 0;

                    s32       i = 0;
                    s32 const n = g_array_size(t->m_data->m_name) - 1;
                    while (i < n)
                    {
                        if (name[i] == 0)
                            break;
                        t->m_data->m_name[i] = name[i];
                        i += 1;
                    }
                    t->m_data->m_name[i] = 0;

                    return t;
                }
                m_data->m_threads_data_pool.deallocate(data);
            }
            return nullptr;
        }

        void threading_t::join(thread_t* t) { t->join(); }

        void threading_t::sleep(u32 ms)
        {
            if (ms > 0)
            {
                struct timespec ts;
                ts.tv_sec  = ms / 1000;
                ts.tv_nsec = (ms % 1000) * 1000000; // Convert milliseconds to nanoseconds
                nanosleep(&ts, nullptr);
            }
        }

        void threading_t::yield()
        {
            // Yield the processor to allow other threads to run
            sched_yield();
        }

        void threading_t::quit()
        {
            thread_data_t* d = thread_data_t::current();
            if (d)
            {
                thread_fn_t* f = d->m_functor;
                if (f)
                {
                    f->quit();
                }
            }
        }

        void threading_t::init_thread_priority(u32* map)
        {
            sched_param sp;
            int         sched_policy = 0;

            pthread_t _thread = pthread_self();
            if (pthread_getschedparam(_thread, &sched_policy, &sp) == 0)
            {
                s32 minPriority     = sched_get_priority_min(sched_policy);
                s32 maxPriority     = sched_get_priority_max(sched_policy);
                s32 priorityQuantum = (maxPriority - minPriority) / 6;
                s32 normalPriority  = (maxPriority - minPriority) / 2;

                map[thread_priority_t::ABOVE_NORMAL] = normalPriority + priorityQuantum;
                map[thread_priority_t::HIGH]         = normalPriority + priorityQuantum;
                map[thread_priority_t::IDLE]         = minPriority;
                map[thread_priority_t::BELOW_NORMAL] = normalPriority - priorityQuantum;
                map[thread_priority_t::LOW]          = normalPriority - priorityQuantum;
                map[thread_priority_t::CRITICAL]     = maxPriority;
                map[thread_priority_t::NORMAL]       = normalPriority;
            }
        }
    } // namespace nthread
} // namespace ncore

#endif
