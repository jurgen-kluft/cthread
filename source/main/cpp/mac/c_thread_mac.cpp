#include "ccore/c_target.h"

#include "cthread/c_thread.h"
#include "cthread/private/c_thread_mac.h"

namespace ncore
{
    void thread_t::create(const char* name, thread_functor* functor, u32 stacksize, thread_priority_t priority) {}
    void thread_t::start() {}
    void thread_t::join() {}
    bool thread_t::join(u32 milliseconds) { return false; }

    void thread_t::set_priority(thread_priority_t p) {}

} // namespace ncore
