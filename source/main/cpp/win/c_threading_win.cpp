#include "ccore/c_target.h"

#ifdef TARGET_PC
#    include "cthread/c_types.h"
#    include "cthread/c_thread.h"
#    include "cthread/c_mutex.h"
#    include "cthread/c_event.h"
#    include "cthread/c_semaphore.h"
#    include "cthread/private/c_thread_win.h"

namespace ncore
{
    thread_t* threading_t::create_thread(const char* name, thread_functor* f, u32 stack_size, thread_priority_t priority)
    {
        thread_data_t* data = (thread_data_t*)m_threads_data_pool.allocate();
        if (data)
        {
            thread_t* t = (thread_t*)m_threads_pool.allocate();
            if (t)
            {
                t->m_data               = data;
                t->m_data->m_functor    = f;
                t->m_data->m_stack_size = stack_size;
                t->m_data->m_priority   = priority;
                t->m_data->m_state      = thread_state_t::CREATED;
                t->m_data->m_idx        = m_threads_pool.ptr2idx(t);
                t->m_data->m_tid        = 0;
                t->m_data->m_name[0]    = 0;

                const char* src         = name;
                char*       dst         = t->m_data->m_name;
                const char* end         = dst + sizeof(t->m_data->m_name) - 1;
                while (*src && dst < end)
                    *dst++ = *src++;
                *dst = 0;

                return t;
            }
            m_threads_data_pool.deallocate(data);
        }
        return nullptr;
    }

    
    unsigned __stdcall __thread_main(void* arg1, void* arg2)
    {
        // Call the real entry point function, passing the provided context.
        thread_t*       t = reinterpret_cast<thread_t*>(arg1);
        thread_data_t*  d = reinterpret_cast<thread_data_t*>(arg2);
        thread_functor* f = d->m_functor;
        {
            f->start(t, d);
            f->run();
            f->exit();
        }
        return 0;
    }

    void threading_t::join(thread_t* t) {}
    bool threading_t::join(thread_t* t, u32 ms) { return false; }

    thread_t*    threading_t::current() 
    { 
        thread_t*    t = nullptr;
        

        return t; 
    }

    void         threading_t::sleep(u32 ms) {}
    void         threading_t::yield() {}
    void         threading_t::exit() {}


    void threading_t::init_thread_priority(u32* map)
    {
        
    }

} // namespace ncore

#endif