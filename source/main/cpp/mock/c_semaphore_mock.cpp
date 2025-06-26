#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"

#if defined(TARGET_TEST)
#    include "cthread/c_semaphore.h"
#    include "cthread/c_threading.h"
#    include "cthread/private/c_thread_mock.h"

namespace ncore
{
    namespace nthread
    {
        bool sema_t::init(sema_data_t* data, s32 n, s32 max)
        {
            m_data = data;
            ASSERT(n >= 0 && max > 0 && n <= max);
            m_data->_sema = 1;
            if (!m_data->_sema)
            {
                // cannot create semaphore
                return false;
            }
            return true;
        }

        void sema_t::release() { threading_t::instance()->destroy(this); }
        void sema_t::signal() {}
        void sema_t::wait() {}
        bool sema_t::try_wait(u32 milliseconds) { return true; }
    } // namespace nthread
} // namespace ncore

#endif
