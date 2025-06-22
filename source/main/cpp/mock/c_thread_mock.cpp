#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"
#include "cbase/c_va_list.h"

#if defined(TARGET_TEST)
#    include "cthread/c_thread.h"
#    include "cthread/c_threading.h"
#    include "cthread/private/c_threading_data.h"
#    include "cthread/private/c_thread_mock.h"

#    include "cthread/c_mutex.h"
#    include "cthread/c_event.h"
#    include "cthread/c_semaphore.h"

namespace ncore
{
    void thread_t::set_priority(thread_priority_t priority)
    {
        if (priority != m_data->m_priority)
        {
            m_data->m_priority = priority;
            if (m_data->m_handle)
            {
                threading_t* threading = threading_t::instance();
                s32 const    p         = threading->m_data->m_thread_priority_map[priority.prio];
            }
        }
    }

    s32 thread_t::create()
    {
        s32 result = 0;
        if (m_data->m_priority == thread_priority_t::HIGH)
        {
            result = thread_priority_t::HIGH;
        }
        else if (m_data->m_priority == thread_priority_t::ABOVE_NORMAL)
        {
            result = thread_priority_t::ABOVE_NORMAL;
        }
        else if (m_data->m_priority == thread_priority_t::NORMAL)
        {
            result = thread_priority_t::NORMAL;
        }
        else if (m_data->m_priority == thread_priority_t::BELOW_NORMAL)
        {
            result = thread_priority_t::BELOW_NORMAL;
        }
        else if (m_data->m_priority == thread_priority_t::LOW)
        {
            result = thread_priority_t::LOW;
        }

        // Under Windows, we don't set the thread affinity and let the OS deal with scheduling
        m_data->m_state  = thread_state_t::RUNNING;
        m_data->m_handle = (hnd_t)1;
        return result;
    }

    void thread_t::start()
    {
        if (get_state() != thread_state_t::RUNNING)
        {
            create();
        }

        thread_data_t* d = get_data();

        // Set the thread ID now that we are in the thread
        d->m_tid = 8;

        thread_fn_t* f = d->m_functor;
        {
            f->start(this, d);
            f->run();
            f->exit();
        }
    }

    s32 thread_t::join()
    {
        if (!m_data->m_handle || m_data->m_state == thread_state_t::STOPPED)
            return 0;
        return 0;
    }

} // namespace ncore

#endif
