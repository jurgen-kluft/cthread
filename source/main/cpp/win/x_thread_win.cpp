#include "xthread/x_thread.h"
#include "xthread/x_thread_functor.h"
#include <process.h>

namespace xcore 
{
	thread_local xthread_impl*	tl_thread_ptr = NULL;


	xthread_impl::xthread_impl(xthread_config const& _config)
		: m_functor(0)
		, m_threadHandle(0)
		, m_threadIdx(0)
		, m_threadTid(0)
		, m_config(_config)
	{
	}


	xthread_impl::~xthread_impl()
	{
		if (m_threadHandle)
			::CloseHandle(m_threadHandle);
	}


	void xthread_impl::thread_set_priority(EThreadPriority priority)
	{
		if (priority != m_config.m_priority)
		{
			m_config.m_priority = priority;
			if (m_threadHandle)
			{
				if (::SetThreadPriority(m_threadHandle, m_config.m_priority) == 0)
				{
					// cannot set thread priority
				}
			}
		}
	}

	void xthread_impl::thread_start(xthread_functor* _f)
	{
		if (thread_get_state() == XTHREAD_STATE_RUNNING)
		{
			// thread already running
		}
		else if (thread_get_state() == XTHREAD_STATE_CREATED)
		{
			// Create a data structure to wrap the data we need to pass to the entry function.
			m_functor = _f;
			ResumeThread(m_threadHandle);
		}
	}

	void xthread_impl::thread_run()
	{
		m_functor->run();
	}

	unsigned __stdcall 	__main_func(void* arg)
	{
		// Call the real entry point function, passing the provided context.
		xthread_impl* t = reinterpret_cast<xthread_impl*>(arg);
		tl_thread_ptr = t;
		{
			t->thread_run();
			t->thread_exit();
		}
		tl_thread_ptr = NULL;
		return 0;
	}

	void xthread_impl::thread_create(xthread_impl* _this)
	{
		u32 threadId;
		xthread_hnd_t thread_hnd = (HANDLE)::_beginthreadex(NULL, _this->m_config.m_stack_size, &__main_func, _this, CREATE_SUSPENDED, &threadId);
		xthread_tid_t thread_tid = static_cast<DWORD>(threadId);

		if (thread_hnd)
		{
			_this->m_threadHandle = thread_hnd;
			_this->m_threadTid = thread_tid;
			_this->m_threadState = XTHREAD_STATE_CREATED;
		}

		if (_this->m_config.m_priority != XTHREAD_PRIORITY_NORMAL && !SetThreadPriority(thread_hnd, _this->m_config.m_priority))
		{
			// cannot set thread priority
		}
	}

	void xthread_impl::thread_exit()
	{
		// Terminating a thread with a call to _endthreadex helps to ensure proper
		// recovery of resources allocated for the thread.
		// http://msdn.microsoft.com/en-us/library/kdzttdcb(v=vs.80).aspx
		_endthreadex(0);
	}

	void xthread_impl::thread_join()
	{
		if (!m_threadHandle)
			return;

		switch (WaitForSingleObject(m_threadHandle, INFINITE))
		{
		case WAIT_OBJECT_0:
			::CloseHandle(m_threadHandle);
			m_threadHandle = 0;
			m_threadState = XTHREAD_STATE_STOPPED;
			return;
		default:
			// cannot join thread
			break;
		}
	}

	EThreadState xthread_impl::thread_get_state() const
	{
		return m_threadState;
	}

	xthread_impl* xthread_impl::thread_current()
	{
		return tl_thread_ptr;
	}

	xthread_tid_t xthread_impl::thread_current_tid()
	{
		return tl_thread_ptr->thread_current_tid();
	}

	xthread_idx_t xthread_impl::thread_current_idx()
	{
		return tl_thread_ptr->thread_current_idx();
	}

} // namespace xcore
