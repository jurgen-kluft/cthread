#ifndef __CMTHREAD_THREAD_MOCK_H__
#define __CMTHREAD_THREAD_MOCK_H__
#include "ccore/c_target.h"

#include "cthread/c_threading.h"
#include "cthread/c_types.h"

namespace ncore
{
    typedef void* hnd_t;

    struct thread_data_t
    {
        hnd_t             m_handle;
        thread_idx_t      m_idx;
        thread_id_t       m_tid;
        thread_state_t    m_state;
        thread_priority_t m_priority;
        u32               m_stack_size;
        thread_fn_t*      m_functor;
        void*             m_arg;
        char              m_name[64];
    };

    struct event_data_t
    {
        s32 m_event;
    };

    struct mutex_data_t
    {
        s32 m_mutex;
    };

    struct sema_data_t
    {
        s32 _sema;
    };

} // namespace ncore

#endif // __CMTHREAD_THREAD_WIN_H__
