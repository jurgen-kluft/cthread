#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ctime/c_datetime.h"

#ifdef TARGET_PC
#    include "cthread/c_threading.h"
#    include "cthread/c_mutex.h"
#    include "cthread/private/c_thread_win.h"

namespace ncore
{
    bool mutex_t::init(mutex_data_t* data)
    {
        m_data = data;

        // the func has a boolean return value under WInnNt/2000/XP but not on Win98
        // the return only checks if the input address of &_cs was valid, so it is
        // safe to omit it.
        InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION*)&m_data->_cs, 4000);
    }

    void mutex_t::release()
    {
        DeleteCriticalSection((CRITICAL_SECTION*)&m_data->_cs);
        threading_t::instance()->destroy_mutex(this);
    }
    void mutex_t::lock() { EnterCriticalSection((CRITICAL_SECTION*)&m_data->_cs); }
    bool mutex_t::tryLock() { return TryEnterCriticalSection((CRITICAL_SECTION*)&m_data->_cs) != 0; }
    void mutex_t::unlock() { LeaveCriticalSection((CRITICAL_SECTION*)&m_data->_cs); }

} // namespace ncore

#endif