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
    void thread_t::destroy()
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
                s32 const    p         = threading->m_thread_priority_map[priority.prio];

                if (::SetThreadPriority(m_data->m_handle, p) == 0)
                {
                    // cannot set thread priority
                }
            }
        }
    }
    static void SetThreadDescription(const char* threadDescription)
    {
        // SetThreadDescription is only available from Windows 10 version 1607 / Windows Server 2016
        //
        // So in order to be compatible with older Windows versions we probe for the API at runtime
        // and call it only if available.

        typedef HRESULT(WINAPI * SetThreadDescriptionFnPtr)(HANDLE hThread, PCWSTR threadDescription);

#    pragma warning(push)
#    pragma warning(disable : 4191) // unsafe conversion from 'type of expression' to 'type required'
        static SetThreadDescriptionFnPtr RealSetThreadDescription = (SetThreadDescriptionFnPtr)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "SetThreadDescription");
#    pragma warning(pop)

        if (RealSetThreadDescription)
        {
            RealSetThreadDescription(::GetCurrentThread(), (PCWSTR)threadDescription);
        }
    }

    void thread_t::create(thread_data_t* data)
    {
        m_data = data;

        DWORD flags = CREATE_SUSPENDED;

        // Without this flag the 'dwStackSize' parameter to CreateThread specifies the "Stack Commit Size"
        // and the "Stack Reserve Size" is set to the value specified at link-time.
        // With this flag the 'dwStackSize' parameter to CreateThread specifies the "Stack Reserve Size"
        // and the “Stack Commit Size” is set to the value specified at link-time.
        // For various reasons (some of which historic) we reserve a large amount of stack space in the
        // project settings. By setting this flag and by specifying 64 kB for the "Stack Commit Size" in
        // the project settings we can create new threads with a much smaller reserved (and committed)
        // stack space. It is very important that the "Stack Commit Size" is set to a small value in
        // the project settings. If it is set to a large value we may be both reserving and committing
        // a lot of memory by setting the STACK_SIZE_PARAM_IS_A_RESERVATION flag. There are some
        // 50 threads allocated for normal game play. If, for instance, the commit size is set to 16 MB
        // then by adding this flag we would be reserving and committing 50 x 16 = 800 MB of memory.
        // On the other hand, if this flag is not set and the "Stack Reserve Size" is set to 16 MB in the
        // project settings, then we would still be reserving 50 x 16 = 800 MB of virtual address space.
        flags |= STACK_SIZE_PARAM_IS_A_RESERVATION;

        DWORD  threadId;
        HANDLE handle = CreateThread(NULL, // LPSECURITY_ATTRIBUTES lpsa, //-V513
                                     data->m_stack_size, (LPTHREAD_START_ROUTINE)data->m_functor, data->m_arg, flags, &threadId);
        if (handle == 0)
        {
            return;
        }

        SetThreadDescription(m_data->m_name);

        if (m_data->m_priority == thread_priority_t::HIGH)
        {
            SetThreadPriority((HANDLE)handle, THREAD_PRIORITY_HIGHEST); //  we better sleep enough to do this
        }
        else if (m_data->m_priority == thread_priority_t::ABOVE_NORMAL)
        {
            SetThreadPriority((HANDLE)handle, THREAD_PRIORITY_ABOVE_NORMAL);
        }
        else if (m_data->m_priority == thread_priority_t::BELOW_NORMAL)
        {
            SetThreadPriority((HANDLE)handle, THREAD_PRIORITY_BELOW_NORMAL);
        }
        else if (m_data->m_priority == thread_priority_t::LOW)
        {
            SetThreadPriority((HANDLE)handle, THREAD_PRIORITY_LOWEST);
        }

        // Under Windows, we don't set the thread affinity and let the OS deal with scheduling
        m_data->m_state  = thread_state_t::SUSPENDED;
        m_data->m_handle = (hnd_t)handle;
    }

    void thread_t::start()
    {
        if (get_state() == thread_state_t::RUNNING)
        {
            // thread already running
        }
        else if (get_state() == thread_state_t::SUSPENDED)
        {
            ResumeThread(m_data->m_handle);
            m_data->m_state = thread_state_t::RUNNING;
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
                m_data->m_state  = thread_state_t::STOPPED;
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