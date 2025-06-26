#include "ccore/c_target.h"

#if defined(TARGET_TEST)
#    include "cthread/c_types.h"
#    include "cthread/c_thread.h"
#    include "cthread/c_mutex.h"
#    include "cthread/c_event.h"
#    include "cthread/c_semaphore.h"
#    include "cthread/private/c_threading_data.h"
#    include "cthread/private/c_thread_mock.h"

namespace ncore
{
    namespace nthread
    {
        thread_t* threading_t::create_thread(const char* name, thread_fn_t* f, thread_priority_t priority, u32 stack_size)
        {
            thread_data_t* data = (thread_data_t*)m_data->m_threads_data_pool.allocate();
            if (data)
            {
                thread_t* t = (thread_t*)m_data->m_threads_pool.allocate();
                if (t)
                {
                    t->m_data               = data;
                    t->m_data->m_functor    = f;
                    t->m_data->m_stack_size = stack_size;
                    t->m_data->m_priority   = priority;
                    t->m_data->m_state      = thread_state_t::CREATED;
                    t->m_data->m_idx        = m_data->m_threads_pool.ptr2idx(t);
                    t->m_data->m_tid        = 0;
                    t->m_data->m_name[0]    = 0;

                    s32       i = 0;
                    s32 const n = g_array_size(t->m_data->m_name) - 1;
                    while (i < n)
                    {
                        t->m_data->m_name[i] = name[i];
                        if (name[i] == 0)
                            break;
                        i += 1;
                    }
                    t->m_data->m_name[i] = 0;
                    return t;
                }
                m_data->m_threads_data_pool.deallocate(data);
            }
            return nullptr;
        }

        void threading_t::join(thread_t* t) { t->join(); }

        thread_t* threading_t::current()
        {
            thread_t* t = nullptr;

            //@TODO: Implement this

            return t;
        }

        void threading_t::sleep(u32 ms)
        {
            //@TODO: Implement this
        }

        void threading_t::yield()
        {
            //@TODO: Implement this
        }

        void threading_t::quit()
        {
            //@TODO: Implement this
        }

        void threading_t::init_thread_priority(u32* map)
        {
            //@TODO: Implement this
        }
    } // namespace nthread
} // namespace ncore

#endif
