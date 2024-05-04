#include "ccore/c_target.h"
#include "ccore/c_debug.h"

#ifdef TARGET_MAC

#    include "cthread/c_semaphore.h"
#    include "cthread/private/c_thread_mac.h"

#include <mach/mach.h>
#include <mach/task.h>

namespace ncore
{
    // Semaphore (Apple iOS and OSX)

    bool sema_t::init(sema_data_t* data, s32 n, s32 max)
    {
        ASSERT(n >= 0 && max > 0 && n <= max);
        sema_data_t* data = (sema_data_t*)m_data;
        semaphore_create(mach_task_self(), &data->_sema, SYNC_POLICY_FIFO, n);
    }

    void sema_t::release()
    {
        sema_data_t* data = (sema_data_t*)m_data;
        semaphore_destroy(mach_task_self(), data->_sema);
        threading_t::instance()->destroy_sema(this);
    }

    void sema_t::signal()
    {
        sema_data_t* data = (sema_data_t*)m_data;
        semaphore_signal(data->_sema);
    }

    void sema_t::wait()
    {
        sema_data_t* data = (sema_data_t*)m_data;
        semaphore_wait(data->_sema);
    }

} // namespace ncore

#endif