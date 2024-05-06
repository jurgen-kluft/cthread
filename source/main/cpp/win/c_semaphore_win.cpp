#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"

#ifdef TARGET_PC
#    include "cthread/c_semaphore.h"
#    include "cthread/c_threading.h"
#    include "cthread/private/c_thread_win.h"

namespace ncore
{
    bool sema_t::init(sema_data_t* data, s32 n, s32 max)
    {
        m_data = data;
        ASSERT(n >= 0 && max > 0 && n <= max);
        m_data->_sema = CreateSemaphoreW(nullptr, n, max, nullptr);
        if (!m_data->_sema)
        {
            // cannot create semaphore
            return false;
        }
        return true;
    }

    void sema_t::release()
    {
        CloseHandle(m_data->_sema);
        threading_t::instance()->destroy(this);
    }

    void sema_t::signal()
    {
        if (!ReleaseSemaphore(m_data->_sema, 1, nullptr))
        {
            // cannot signal semaphore
        }
    }

    void sema_t::wait()
    {
        switch (WaitForSingleObject(m_data->_sema, INFINITE))
        {
            case WAIT_OBJECT_0: return;
            default:
                // wait for semaphore failed
                break;
        }
    }

    bool sema_t::try_wait(u32 milliseconds)
    {
        switch (WaitForSingleObject(m_data->_sema, milliseconds))
        {
            case WAIT_OBJECT_0: return true;
            default:
                // wait for semaphore failed
                break;
        }
        return false;
    }

} // namespace ncore

#endif