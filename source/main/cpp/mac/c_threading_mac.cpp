#include "ccore/c_target.h"

#ifdef TARGET_MAC
#    include "cthread/c_event.h"
#    include "cthread/private/c_thread_mac.h"

namespace ncore
{
    threading_t* threading_t::create(alloc_t* allocator, u32 max_threads, u32 max_mutex, u32 max_event, u32 max_semaphore) { return nullptr; }

    void threading_t::destroy(threading_t*& t) {}

    void         threading_t::set_instance(threading_t* instance) {}
    threading_t* threading_t::instance() { return nullptr; }

    thread_t* threading_t::create_thread(const char* name, void* arg, thread_functor* f, u32 stack_size, thread_priority_t priority) { return nullptr; }
    mutex_t*  threading_t::create_mutex() { return nullptr; }
    event_t*  threading_t::create_event(const char* name, bool autoReset) { return nullptr; }
    sema_t*   threading_t::create_sema(s32 initial_count, s32 max_count) { return nullptr; }

    void threading_t::destroy(thread_t*) {}
    void threading_t::destroy(mutex_t*) {}
    void threading_t::destroy(event_t*) {}
    void threading_t::destroy(sema_t*) {}

    void threading_t::join(thread_t*) {}
    bool threading_t::join(thread_t*, u32 milliseconds) {}

    thread_t*    threading_t::current() { return nullptr; }
    thread_id_t  threading_t::current_tid() { return 0; }
    thread_idx_t threading_t::current_idx() { return 0; }
    void         threading_t::sleep(u32 milliseconds) {}
    void         threading_t::yield() {}
    void         threading_t::exit() {}

} // namespace ncore

#endif