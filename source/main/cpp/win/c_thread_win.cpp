#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"
#include "cbase/c_va_list.h"

#ifdef TARGET_PC
#    include "cthread/c_thread.h"
#    include "cthread/c_threading.h"
#    include "cthread/private/c_threading.h"

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
        if (m_data->m_threadHandle)
            ::CloseHandle(m_data->m_threadHandle);
    }

    void thread_t::set_priority(thread_priority_t priority)
    {
        if (priority != m_data->m_priority)
        {
            m_data->m_priority = priority;
            if (m_data->m_threadHandle)
            {
                if (::SetThreadPriority(m_data->m_threadHandle, m_data->m_priority.to_platform_value()) == 0)
                {
                    // cannot set thread priority
                }
            }
        }
    }

    void thread_t::start()
    {
        if (get_state() == STATE_RUNNING)
        {
            // thread already running
        }
        else if (get_state() == STATE_CREATED)
        {
            // Create a data structure to wrap the data we need to pass to the entry function.
            ResumeThread(m_data->m_threadHandle);
        }
    }

    unsigned __stdcall __main_func(void* arg)
    {
        // Call the real entry point function, passing the provided context.
        thread_ata_t* t = reinterpret_cast<thread_data_t*>(arg);
        {
            t->run();
            t->exit();
        }
        return 0;
    }

    void thread_t::join()
    {
        thread_win_t* data = (thread_win_t*)this;
        if (!m_data->m_threadHandle)
            return;

        switch (WaitForSingleObject(m_data->m_threadHandle, INFINITE))
        {
            case WAIT_OBJECT_0:
            {
                ::CloseHandle(m_data->m_threadHandle);
                m_data->m_threadHandle = 0;
                m_data->m_threadState  = STATE_STOPPED;
                return;
            }
            default:
                // cannot join thread
                break;
        }
    }

    bool thread_t::join(u32 milliseconds)
    {
        thread_win_t* data = (thread_win_t*)this;
        if (!m_data->m_threadHandle)
            return true;

        switch (WaitForSingleObject(m_data->m_threadHandle, milliseconds))
        {
            case WAIT_OBJECT_0:
                ::CloseHandle(m_data->m_threadHandle);
                m_data->m_threadHandle = 0;
                m_data->m_threadState  = STATE_STOPPED;
                return true;
            default:
                // cannot join thread
                break;
        }
        return false;
    }

    u32               thread_t::get_stacksize() const { return m_data->m_stack_size; }
    thread_state_t    thread_t::get_state() const { return m_data->m_threadState; }
    thread_priority_t thread_t::get_priority() const { return m_data->m_priority; }

} // namespace ncore

#endif