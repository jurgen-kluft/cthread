#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_debug.h"

#ifdef TARGET_PC
#include "xthread/x_semaphore.h"
#include "xthread/x_threading.h"

#include <windows.h>
#include <stdio.h>

namespace xcore 
{
	class xsemaphore_win : public xsemaphore
	{
	public:
		xsemaphore_win(s32 n) : xsemaphore(n) {}
		xsemaphore_win(s32 n, s32 max) : xsemaphore(n, max) {}

		XCORE_CLASS_PLACEMENT_NEW_DELETE

		void* _sema;
	};

	xsemaphore::xsemaphore(s32 n, s32 max)
	{
		ASSERT(n >= 0 && max > 0 && n <= max);
		xsemaphore_win* data = (xsemaphore_win*)this;
		data->_sema = CreateSemaphoreW(NULL, n, max, NULL);
		if (!data->_sema)
		{
			// cannot create semaphore
		}
	}

	xsemaphore::~xsemaphore()
	{
		xsemaphore_win* data = (xsemaphore_win*)this;
		CloseHandle(data->_sema);
	}

	void xsemaphore::signal()
	{
		xsemaphore_win* data = (xsemaphore_win*)this;
		if (!ReleaseSemaphore(data->_sema, 1, NULL))
		{
			// cannot signal semaphore
		}
	}

	void xsemaphore::wait()
	{
		xsemaphore_win* data = (xsemaphore_win*)this;
		switch (WaitForSingleObject(data->_sema, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		default:
			// wait for semaphore failed
			break;
		}
	}

	bool	xsemaphore::try_wait(u32 milliseconds)
	{
		xsemaphore_win* data = (xsemaphore_win*)this;
		switch (WaitForSingleObject(data->_sema, milliseconds))
		{
		case WAIT_OBJECT_0:
			return true;
		default:
			// wait for semaphore failed
			break;
		}
		return false;
	}

	class xsemaphores_data
	{
	public:
		xsemaphore*     m_semaphores;
		xfsadexed_array m_alloc;
		XCORE_CLASS_PLACEMENT_NEW_DELETE
	};


	xsemaphores_data*	gCreateSemaphoresData(xalloc* alloc, u32 max_semaphores)
	{
		xsemaphores_data* semas = alloc->construct<xsemaphores_data>();
		xsemaphore_win* sema_array = (xsemaphore_win*)alloc->allocate(sizeof(xsemaphore_win) * max_semaphores);
		semas->m_alloc = xfsadexed_array(sema_array, sizeof(xsemaphore_win), max_semaphores);
		return semas;
	}

	xsemaphore* xthreading::create_semaphore(s32 initial_count, s32 max_count)
	{
		xsemaphore_win* win_sema = m_semaphores->m_alloc.construct<xsemaphore_win>(initial_count, max_count);
		return win_sema;
	}

	void xthreading::destroy_semaphore(xsemaphore* sema)
	{
		xsemaphore_win* win_sema = (xsemaphore_win*)sema;
		m_semaphores->m_alloc.destruct(win_sema);
	}


} // namespace xcore

#endif