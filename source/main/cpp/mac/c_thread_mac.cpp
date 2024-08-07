#include "ccore/c_target.h"

#include "cthread/c_thread.h"
#include "cthread/private/c_thread_mac.h"

namespace ncore
{
    void thread_t::create(thread_data_t* data) { m_data = data; }

    void thread_t::destroy() {}

    void thread_t::set_priority(thread_priority_t priority)
    {
        if (priority != m_data->m_priority)
        {
            m_data->m_priority = priority;
            if (m_data->m_handle)
            {
                threading_t* threading = threading_t::instance();
                u32 const    prio      = threading->m_thread_priority_map[m_data->m_priority.prio];

                {
                    // cannot set thread priority
                }
            }
        }
    }

    void thread_t::start()
    {
        if (m_data->m_state == thread_state_t::RUNNING)
        {
            // thread already running
        }
        else if (m_data->m_state == thread_state_t::CREATED)
        {
            // Create a data structure to wrap the data we need to pass to the entry function.
            // ResumeThread(m_data->m_handle);
        }
    }

    void thread_t::suspend()
    {
        if (m_data->m_state == thread_state_t::RUNNING)
        {
            //SuspendThread(m_data->m_handle);
            m_data->m_state = thread_state_t::SUSPENDED;
        }
    }

    void thread_t::resume()
    {
        if (m_data->m_state == thread_state_t::SUSPENDED)
        {
            //ResumeThread(m_data->m_handle);
            m_data->m_state = thread_state_t::RUNNING;
        }
    }

    u32 s__main_func(void* arg1, void* arg2)
    {
        // Call the real entry point function, passing the provided context.
        thread_t*      t = reinterpret_cast<thread_t*>(arg1);
        thread_data_t* d = reinterpret_cast<thread_data_t*>(arg2);
        thread_functor* f = d->m_functor;
        {
            f->start(t, d);
            f->run();
            f->exit();
        }
        return 0;
    }

    void thread_t::join()
    {
        if (!m_data->m_handle)
            return;
    }

    bool thread_t::join(u32 milliseconds)
    {
        if (!m_data->m_handle)
            return true;

        return false;
    }

} // namespace ncore
