#ifndef __CMTHREAD_THREAD_WIN_H__
#define __CMTHREAD_THREAD_WIN_H__
#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/c_types.h"

#ifdef TARGET_PC

#    include <Windows.h>

namespace ncore
{
    struct event_data_t
    {
        void* m_event;
    };

    struct mutex_data_t
    {
        CRITICAL_SECTION _cs;
    };

    struct sema_data_t
    {
        void* _sema;
    };

} // namespace ncore

#endif // TARGET_PC
#endif // __CMTHREAD_THREAD_WIN_H__
