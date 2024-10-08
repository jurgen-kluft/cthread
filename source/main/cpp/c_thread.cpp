#include "ccore/c_target.h"
#include "cbase/c_runes.h"
#include "cbase/c_va_list.h"
#include "cbase/c_printf.h"

#include "cthread/c_thread.h"

#ifdef TARGET_MAC
#    include "cthread/private/c_thread_mac.h"
#endif

#ifdef TARGET_PC
#    include "cthread/private/c_thread_win.h"
#endif

#include <atomic>

namespace ncore
{
    enum e_config
    {
        DEFAULT_STACKSIZE = 256 * 1024,
        DEFAULT_PRIORITY  = thread_priority_t::NORMAL
    };

    thread_id_t       thread_t::get_tid() const { return m_data->m_tid; }
    thread_idx_t      thread_t::get_idx() const { return m_data->m_idx; }
    const char*       thread_t::get_name() const { return m_data->m_name; }
    thread_priority_t thread_t::get_priority() const { return m_data->m_priority; }
    thread_state_t    thread_t::get_state() const { return m_data->m_state; }
    thread_data_t*    thread_t::get_data() const { return m_data; }
    bool              thread_t::is_running() const { return m_data->m_state == thread_state_t::RUNNING; }
    u32               thread_t::get_stacksize() const { return m_data->m_stack_size; }

    u32               thread_t::default_stacksize() { return e_config::DEFAULT_STACKSIZE; }
    thread_priority_t thread_t::default_priority() { return e_config::DEFAULT_PRIORITY; }

} // namespace ncore
