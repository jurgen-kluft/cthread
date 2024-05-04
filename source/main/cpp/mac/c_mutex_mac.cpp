#include "ctime/c_datetime.h"

#ifdef TARGET_MAC
#    include "cthread/c_threading.h"
#    include "cthread/c_mutex.h"
#    include "cthread/private/c_thread_mac.h"

namespace ncore
{
    bool mutex_t::init(mutex_data_t* data)
    {
        m_data = data;
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutex_init(&m_data->_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
        return true;
    }

    void mutex_t::release()
    {
        pthread_mutex_destroy(&m_data->_mutex);
        threading_t::instance()->destroy(this);
    }

    void mutex_t::lock()
    {
        pthread_mutex_lock(&m_data->_mutex);
    }

    bool mutex_t::tryLock()
    {
        return pthread_mutex_trylock(&m_data->_mutex) == 0;
    }

    void mutex_t::unlock()
    {
        pthread_mutex_unlock(&m_data->_mutex);
    }
} // namespace ncore

#endif