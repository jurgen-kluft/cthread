#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"
#include "cbase/c_va_list.h"

#ifdef TARGET_PC
#    include "cthread/c_thread.h"
#    include "cthread/c_threading.h"
#    include "cthread/private/c_thread_win.h"

#    include "cthread/c_mutex.h"
#    include "cthread/c_event.h"
#    include "cthread/c_semaphore.h"

#    include <process.h>
#    include <Windows.h>
#    include <atomic>

namespace ncore
{
    void thread_t::init(thread_data_t* data) { m_data = data; }

    void thread_t::release()
    {
        if (m_data->m_handle)
            ::CloseHandle(m_data->m_handle);
    }

    void thread_t::set_priority(thread_priority_t priority)
    {
        if (priority != m_data->m_priority)
        {
            m_data->m_priority = priority;
            if (m_data->m_handle)
            {
                threading_t* threading = threading_t::instance();
                s32 const p = threading->m_thread_priority_map[priority.prio];

                if (::SetThreadPriority(m_data->m_handle, p) == 0)
                {
                    // cannot set thread priority
                }
            }
        }
    }

    void thread_t::start()
    {
        if (get_state() == thread_state_t::RUNNING)
        {
            // thread already running
        }
        else if (get_state() == thread_state_t::CREATED)
        {
            // Create a data structure to wrap the data we need to pass to the entry function.
            ResumeThread(m_data->m_handle);
        }
    }

    void thread_t::suspend()
    {
        if (get_state() == thread_state_t::RUNNING)
        {
			SuspendThread(m_data->m_handle);
            m_data->m_state = thread_state_t::SUSPENDED;
		}
	}

    void thread_t::resume()
    {
		if (get_state() == thread_state_t::SUSPENDED) 
        {
            ResumeThread(m_data->m_handle);
			m_data->m_state = thread_state_t::RUNNING;
        }
    }

    void thread_t::join()
    {
        if (!m_data->m_handle)
            return;

        switch (WaitForSingleObject(m_data->m_handle, INFINITE))
        {
            case WAIT_OBJECT_0:
            {
                ::CloseHandle(m_data->m_handle);
                m_data->m_handle = 0;
                m_data->m_state = thread_state_t::STOPPED;
                return;
            }
            default:
                // cannot join thread
                break;
        }
    }

    bool thread_t::join(u32 milliseconds)
    {
        if (!m_data->m_handle)
            return true;

        switch (WaitForSingleObject(m_data->m_handle, milliseconds))
        {
            case WAIT_OBJECT_0:
                ::CloseHandle(m_data->m_handle);
                m_data->m_handle = 0;
                m_data->m_state  = thread_state_t::STOPPED;
                return true;
            default:
                // cannot join thread
                break;
        }
        return false;
    }

} // namespace ncore

#endif