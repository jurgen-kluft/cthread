#include "ccore/c_target.h"
#include "cbase/c_runes.h"
#include "cbase/c_va_list.h"
#include "cbase/c_printf.h"

#include "cthread/c_thread.h"

#if defined(TARGET_MAC) && !defined(TARGET_TEST)
#    include "cthread/private/c_thread_mac.h"
#    include <atomic>
#endif
#if defined(TARGET_PC) && !defined(TARGET_TEST)
#    include "cthread/private/c_thread_win.h"
#    include <atomic>
#endif
#if defined(TARGET_TEST)
#    include "cthread/private/c_thread_mock.h"
#endif

namespace ncore
{
    namespace nthread
    {
        enum e_config
        {
            DEFAULT_STACKSIZE = 256 * 1024,
            DEFAULT_PRIORITY  = npriority::NORMAL
        };

        id_t        thread_get_tid(thread_t* t) { return t->m_data->m_tid; }
        index_t     thread_get_idx(thread_t* t) { return t->m_data->m_idx; }
        const char* thread_get_name(thread_t* t) { return t->m_data->m_name; }
        priority_t  thread_get_priority(thread_t* t) { return t->m_data->m_priority; }
        state_t     thread_get_state(thread_t* t) { return t->m_data->m_state; }
        system_t*   thread_get_system(thread_t* t) { return s_system; }
        bool        thread_is_running(thread_t* t) { return t->m_data->m_state == nstate::RUNNING; }
        u32         thread_get_stacksize(thread_t* t) { return t->m_data->m_stack_size; }

        u32        thread_default_stacksize() { return e_config::DEFAULT_STACKSIZE; }
        priority_t thread_default_priority() { return e_config::DEFAULT_PRIORITY; }
    } // namespace nthread
} // namespace ncore
