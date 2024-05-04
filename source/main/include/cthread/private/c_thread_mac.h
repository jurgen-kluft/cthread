#ifndef __CMTHREAD_THREAD_OSX_H__
#define __CMTHREAD_THREAD_OSX_H__
#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/c_types.h"

#include <thread>
#include <chrono>
#include <mach/semaphore.h>

namespace ncore
{
    struct event_data_t
    {
        pthread_mutex_t _mutex;
        pthread_cond_t  _cond;
        bool            _triggered;
    };

    struct mutex_data_t
    {
        pthread_mutex_t _mutex;
    };

    struct sema_data_t
    {
        semaphore_t _sema;
    };

} // namespace ncore

#endif // __CMTHREAD_THREAD_OSX_H__
