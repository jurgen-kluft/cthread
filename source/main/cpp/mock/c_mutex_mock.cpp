#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cthread/c_mutex.h"

#if defined(TARGET_TEST)
#    include "cthread/c_threading.h"
#    include "cthread/private/c_thread_mock.h"

namespace ncore
{
    bool mutex_t::init(mutex_data_t* data)
    {
        m_data = data;

        return true;
    }

    void mutex_t::release() { threading_t::instance()->destroy(this); }

    void mutex_t::lock() {}
    bool mutex_t::tryLock() { return true; }
    void mutex_t::unlock() {}

} // namespace ncore

#endif
