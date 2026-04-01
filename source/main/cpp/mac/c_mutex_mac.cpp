#include "cthread/c_mutex.h"

#if defined(TARGET_MAC) && !defined(TARGET_TEST)
#    include "cthread/c_threading.h"
#    include "cthread/private/c_thread_mac.h"

namespace ncore
{
    namespace nthread
    {
        bool mutex_data_init(mutex_data_t* data)
        {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutex_init(&data->_mutex, &attr);
            pthread_mutexattr_destroy(&attr);
            return true;
        }

        void mutex_data_release(mutex_data_t* data)
        {
            pthread_mutex_destroy(&data->_mutex);
        }

        void mutex_t::lock() { pthread_mutex_lock(&m_data->_mutex); }
        bool mutex_t::tryLock() { return pthread_mutex_trylock(&m_data->_mutex) == 0; }
        void mutex_t::unlock() { pthread_mutex_unlock(&m_data->_mutex); }
    } // namespace nthread
} // namespace ncore

#endif
