#include "xbase/x_target.h"

#ifdef TARGET_PC
#include "xthread/x_thread.h"
#include "xthread/x_thread_functor.h"

#include <process.h>
#include <Windows.h>

namespace xcore 
{
	thread_local ithread*	tl_thread_ptr = NULL;

	const s32 ithread::e_priority::LOWEST = THREAD_PRIORITY_LOWEST;
	const s32 ithread::e_priority::LOW = THREAD_PRIORITY_BELOW_NORMAL;
	const s32 ithread::e_priority::NORMAL = THREAD_PRIORITY_NORMAL;
	const s32 ithread::e_priority::HIGH = THREAD_PRIORITY_ABOVE_NORMAL;
	const s32 ithread::e_priority::HIGHEST = THREAD_PRIORITY_HIGHEST;

	ithread::ithread(config const& _config)
		: m_functor(0)
		, m_threadHandle(0)
		, m_threadIdx(0)
		, m_threadTid(0)
		, m_config(_config)
	{
	}


	ithread::~ithread()
	{
		if (m_threadHandle)
			::CloseHandle(m_threadHandle);
	}


	void ithread::thread_set_priority(e_priority priority)
	{
		if (priority != m_config.m_priority)
		{
			m_config.m_priority = priority;
			if (m_threadHandle)
			{
				if (::SetThreadPriority(m_threadHandle, m_config.m_priority.prio) == 0)
				{
					// cannot set thread priority
				}
			}
		}
	}

	void ithread::thread_start(xthread_functor* _f)
	{
		if (thread_get_state() == STATE_RUNNING)
		{
			// thread already running
		}
		else if (thread_get_state() == STATE_CREATED)
		{
			// Create a data structure to wrap the data we need to pass to the entry function.
			m_functor = _f;
			ResumeThread(m_threadHandle);
		}
	}

	void ithread::thread_run()
	{
		m_functor->run();
	}

	unsigned __stdcall 	__main_func(void* arg)
	{
		// Call the real entry point function, passing the provided context.
		ithread* t = reinterpret_cast<ithread*>(arg);
		tl_thread_ptr = t;
		{
			t->thread_run();
			t->thread_exit();
		}
		tl_thread_ptr = NULL;
		return 0;
	}

	void ithread::thread_create(ithread* _this)
	{
		u32 threadId;
		hnd_t thread_hnd = (HANDLE)::_beginthreadex(NULL, _this->m_config.m_stack_size, &__main_func, _this, CREATE_SUSPENDED, &threadId);
		tid_t thread_tid = static_cast<DWORD>(threadId);

		if (thread_hnd)
		{
			_this->m_threadHandle = thread_hnd;
			_this->m_threadTid = thread_tid;
			_this->m_threadState = STATE_CREATED;
		}

		if (_this->m_config.m_priority != e_priority::NORMAL && !SetThreadPriority(thread_hnd, _this->m_config.m_priority.prio))
		{
			// cannot set thread priority
		}
	}

	void ithread::thread_exit()
	{
		// Terminating a thread with a call to _endthreadex helps to ensure proper
		// recovery of resources allocated for the thread.
		// http://msdn.microsoft.com/en-us/library/kdzttdcb(v=vs.80).aspx
		_endthreadex(0);
	}

	void ithread::thread_join()
	{
		if (!m_threadHandle)
			return;

		switch (WaitForSingleObject(m_threadHandle, INFINITE))
		{
		case WAIT_OBJECT_0:
			::CloseHandle(m_threadHandle);
			m_threadHandle = 0;
			m_threadState = STATE_STOPPED;
			return;
		default:
			// cannot join thread
			break;
		}
	}

	bool ithread::thread_join(u32 milliseconds)
	{
		if (!m_threadHandle)
			return true;

		switch (WaitForSingleObject(m_threadHandle, milliseconds))
		{
		case WAIT_OBJECT_0:
			::CloseHandle(m_threadHandle);
			m_threadHandle = 0;
			m_threadState = STATE_STOPPED;
			return true;
		default:
			// cannot join thread
			break;
		}
		return false;
	}

	u32				ithread::thread_get_stacksize() const
	{
		return m_config.m_stack_size;
	}

	ithread::e_state ithread::thread_get_state() const
	{
		return m_threadState;
	}

	ithread* ithread::thread_current()
	{
		return tl_thread_ptr;
	}

	ithread::tid_t ithread::thread_current_tid()
	{
		ithread* thread = thread_current();
		return thread->thread_get_tid();
	}

	ithread::idx_t ithread::thread_current_idx()
	{
		ithread* thread = thread_current();
		return thread->thread_get_idx();
	}

	void ithread::thread_sleep(u32 milliseconds)
	{
		::Sleep(DWORD(milliseconds));
	}

	void ithread::thread_yield()
	{
		::Sleep(0);
	}

} // namespace xcore

#endif