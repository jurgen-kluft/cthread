#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_runes.h"
#include "cbase/c_printf.h"
#include "cbase/c_va_list.h"

#ifdef TARGET_PC
#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "cthread/private/c_threading.h"

#include "cthread/c_mutex.h"
#include "cthread/c_event.h"
#include "cthread/c_semaphore.h"

#include <process.h>
#include <Windows.h>
#include <atomic>

namespace ncore
{
    const s32 cthread::e_priority::LOWEST  = THREAD_PRIORITY_LOWEST;
    const s32 cthread::e_priority::LOW    = THREAD_PRIORITY_BELOW_NORMAL;
    const s32 cthread::e_priority::NORMAL  = THREAD_PRIORITY_NORMAL;
    const s32 cthread::e_priority::HIGH    = THREAD_PRIORITY_ABOVE_NORMAL;
    const s32 cthread::e_priority::HIGHEST = THREAD_PRIORITY_HIGHEST;

    typedef void* hnd_t;

    class xthread_win : public cthread
    {
    public:
		xthread_win() : cthread() {}  
		xthread_win(const char* name) : cthread(name) {}
		xthread_win(const char* name, e_priority p, u32 stack_size) : cthread(name, p, stack_size) {}

		~xthread_win() {}

        hnd_t               m_threadHandle;
        cthread::idx_t      m_threadIdx;
        cthread::tid_t      m_threadTid;
        cthread::e_state    m_threadState;
        cthread::e_priority m_priority;
        u32                 m_stack_size;
        xthread_functor*    m_functor;
        void*               m_arg;
        cthread             m_thread;
        char                m_name[64];

		DCORE_CLASS_PLACEMENT_NEW_DELETE

        void run() { m_functor->run(); }
        void exit()
        {
            // Terminating a thread with a call to _endthreadex helps to ensure proper
            // recovery of resources allocated for the thread.
            // http://msdn.microsoft.com/en-us/library/kdzttdcb(v=vs.80).aspx
            _endthreadex(0);
        }
    };
    thread_local xthread_win* tl_thread_ptr = nullptr;

    cthread::~cthread()
    {
        xthread_win* data = (xthread_win*)this;
        if (data->m_threadHandle)
            ::CloseHandle(data->m_threadHandle);
    }

    void cthread::set_priority(e_priority priority)
    {
        xthread_win* data = (xthread_win*)this;
        if (priority != data->m_priority)
        {
            data->m_priority = priority;
            if (data->m_threadHandle)
            {
                if (::SetThreadPriority(data->m_threadHandle, data->m_priority.prio) == 0)
                {
                    // cannot set thread priority
                }
            }
        }
    }

    void cthread::start()
    {
        xthread_win* data = (xthread_win*)this;
        if (get_state() == STATE_RUNNING)
        {
            // thread already running
        }
        else if (get_state() == STATE_CREATED)
        {
            // Create a data structure to wrap the data we need to pass to the entry function.
            ResumeThread(data->m_threadHandle);
        }
    }

    unsigned __stdcall __main_func(void* arg)
    {
        // Call the real entry point function, passing the provided context.
        xthread_win* t = reinterpret_cast<xthread_win*>(arg);
        tl_thread_ptr   = t;
        {
            t->run();
            t->exit();
        }
        tl_thread_ptr = nullptr;
        return 0;
    }

    void cthread::join()
    {
        xthread_win* data = (xthread_win*)this;
        if (!data->m_threadHandle)
            return;

        switch (WaitForSingleObject(data->m_threadHandle, INFINITE))
        {
            case WAIT_OBJECT_0:
            {
                ::CloseHandle(data->m_threadHandle);
                data->m_threadHandle = 0;
                data->m_threadState  = STATE_STOPPED;
                return;
            }
            default:
                // cannot join thread
                break;
        }
    }

    bool cthread::join(u32 milliseconds)
    {
        xthread_win* data = (xthread_win*)this;
        if (!data->m_threadHandle)
            return true;

        switch (WaitForSingleObject(data->m_threadHandle, milliseconds))
        {
            case WAIT_OBJECT_0:
                ::CloseHandle(data->m_threadHandle);
                data->m_threadHandle = 0;
                data->m_threadState  = STATE_STOPPED;
                return true;
            default:
                // cannot join thread
                break;
        }
        return false;
    }

    u32 cthread::get_stacksize() const 
	{
        xthread_win* data = (xthread_win*)this;
        return data->m_stack_size; 
	}

    cthread::e_state cthread::get_state() const 
	{
        xthread_win* data = (xthread_win*)this;
        return data->m_threadState; 
	}

    cthread* cthread::current() { return &tl_thread_ptr->m_thread; }

    cthread::tid_t cthread::current_tid()
    {
        return tl_thread_ptr->m_threadTid;
    }

    cthread::idx_t cthread::current_idx()
    {
		return tl_thread_ptr->m_threadIdx; 
	}

    void cthread::sleep(u32 milliseconds) { ::Sleep(DWORD(milliseconds)); }
    void cthread::yield() { ::Sleep(0); }

	static void sMakeName(ascii::runes& str, cthread::tid_t id) { ascii::sprintf(str, ascii::crunes("#%d"), va_t(id)); }

    static s32 sUniqueId()
    {
        static std::atomic<int> count(0);
        return count++;
    }

    cthread::cthread()
    {
        xthread_win* data = (xthread_win*)this;
        data->m_threadTid = (sUniqueId());
        data->m_name[0]    = '\0';
        data->m_name[1]    = '\0';
        ascii::runes name((ascii::prune)data->m_name, (ascii::prune)data->m_name, &data->m_name[sizeof(data->m_name) - 1]);
        sMakeName(name, data->m_threadTid);
    }

    cthread::cthread(const char* _name)
    {
        xthread_win* data = (xthread_win*)this;
        data->m_threadTid = (sUniqueId());
        ascii::runes name((ascii::prune)data->m_name, (ascii::prune)data->m_name , &data->m_name[sizeof(data->m_name) - 1]);
        utf::copy(_name, name);
    }


    cthread::e_priority cthread::get_priority() const 
	{
        xthread_win* data = (xthread_win*)this;
        return data->m_priority;
	}

	

	// ----------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------
	class xthreads_data
	{
	public:
		xthread_win*   m_threads;
		fsadexed_array_t m_alloc;
		DCORE_CLASS_PLACEMENT_NEW_DELETE
	};

	xthreads_data*		gCreateThreadsData(alloc_t* alloc, u32 max_threads)
	{
		xthreads_data* threads = alloc->construct<xthreads_data>();
		xthread_win* threads_array = (xthread_win*)alloc->allocate(sizeof(xthread_win) * max_threads);
		threads->m_alloc = fsadexed_array_t(threads_array, sizeof(xthread_win), max_threads);
		return threads;

	}

	xthreading::xthreading()
		: m_threads(nullptr)
		, m_events(nullptr)
		, m_mutexes(nullptr)
		, m_semaphores(nullptr)
	{

	}

	xthreading::~xthreading()
	{

	}

    cthread* xthreading::create_thread(const char* name, void* arg, xthread_functor* f, u32 stack_size, cthread::e_priority priority)
    {
        xthread_win* thread_win = m_threads->m_alloc.construct<xthread_win>();
        thread_win->m_arg        = arg;
        thread_win->m_functor    = f;
        thread_win->m_thread     = cthread(name, priority, stack_size);
        cthread* thread    = &thread_win->m_thread;

        u32            threadId   = 0;
        hnd_t          thread_hnd = (HANDLE)::_beginthreadex(nullptr, stack_size, &__main_func, thread_win, CREATE_SUSPENDED, &threadId);
        cthread::tid_t thread_tid = static_cast<DWORD>(threadId);

        if (thread_hnd)
        {
			thread_win->m_threadHandle = thread_hnd;
			thread_win->m_threadTid    = thread_tid;
			thread_win->m_threadState  = cthread::STATE_CREATED;
        }

        if (thread_win->m_priority.prio != cthread::e_priority::NORMAL && !SetThreadPriority(thread_hnd, thread_win->m_priority.prio))
        {
            // cannot set thread priority
        }

        return thread;
    }

    void xthreading::destroy_thread(cthread* thread)
    {
		xthread_win* thread_win = (xthread_win*)thread;
		m_threads->m_alloc.destruct(thread_win);
    }

} // namespace ncore

#endif