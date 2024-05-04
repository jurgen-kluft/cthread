#include "ccore/c_target.h"

#ifdef TARGET_MAC
#    include "cthread/c_types.h"
#    include "cthread/c_thread.h"
#    include "cthread/c_mutex.h"
#    include "cthread/c_event.h"
#    include "cthread/c_semaphore.h"
#    include "cthread/private/c_thread_mac.h"

namespace ncore
{
    thread_t* threading_t::create_thread(const char* name, thread_functor* f, u32 stack_size, thread_priority_t priority)
    {
        thread_data_t* data = (thread_data_t*)m_threads_data_pool.allocate();
        if (data)
        {
            thread_t* t = (thread_t*)m_threads_pool.allocate();
            if (t)
            {
                t->m_data               = data;
                t->m_data->m_functor    = f;
                t->m_data->m_stack_size = stack_size;
                t->m_data->m_priority   = priority;
                t->m_data->m_state      = thread_state_t::CREATED;
                t->m_data->m_idx        = m_threads_pool.ptr2idx(t);
                t->m_data->m_tid        = 0;
                t->m_data->m_name[0]    = 0;
                if (name)
                    strncpy(t->m_data->m_name, name, 63);
                return t;
            }
            m_threads_data_pool.deallocate(data);
        }
        return nullptr;
    }

    void threading_t::join(thread_t* t) {}
    bool threading_t::join(thread_t* t, u32 ms) { return false; }

    thread_t*    threading_t::current() { return nullptr; }
    thread_id_t  threading_t::current_tid() { return 0; }
    thread_idx_t threading_t::current_idx() { return 0; }
    void         threading_t::sleep(u32 ms) {}
    void         threading_t::yield() {}
    void         threading_t::exit() {}

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

} // namespace ncore

#endif