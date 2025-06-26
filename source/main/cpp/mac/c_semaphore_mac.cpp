#include "ccore/c_target.h"
#include "ccore/c_debug.h"

#if defined(TARGET_MAC) && !defined(TARGET_TEST)

#    include "cthread/c_semaphore.h"
#    include "cthread/private/c_thread_mac.h"

#    include <mach/mach.h>
#    include <mach/task.h>

namespace ncore
{
    namespace nthread
    {
        // Semaphore (Apple iOS and OSX)

        bool sema_t::init(sema_data_t* data, s32 n, s32 max)
        {
            ASSERT(n >= 0 && max > 0 && n <= max);
            semaphore_create(mach_task_self(), &m_data->_sema, SYNC_POLICY_FIFO, n);
            return true;
        }

        void sema_t::release()
        {
            semaphore_destroy(mach_task_self(), m_data->_sema);
            threading_t::instance()->destroy(this);
        }

        void sema_t::signal() { semaphore_signal(m_data->_sema); }

        void sema_t::wait()
        {
            sema_data_t* data = (sema_data_t*)m_data;
            semaphore_wait(m_data->_sema);
        }

        bool sema_t::try_wait(u32 milliseconds)
        {
            mach_timespec_t ts;
            ts.tv_sec  = milliseconds / 1000;
            ts.tv_nsec = (milliseconds % 1000) * 1000000;
            return semaphore_timedwait(m_data->_sema, ts) == KERN_SUCCESS;
        }
    } // namespace nthread
} // namespace ncore

#endif
