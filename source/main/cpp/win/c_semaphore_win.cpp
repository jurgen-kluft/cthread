#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"

#ifdef TARGET_PC
#include "cthread/c_semaphore.h"
#include "cthread/c_threading.h"

#include <windows.h>
#include <stdio.h>

namespace ncore 
{
	class xsemaphore_win : public xsemaphore
	{
	public:
		xsemaphore_win(s32 n) : xsemaphore(n) {}
		xsemaphore_win(s32 n, s32 max) : xsemaphore(n, max) {}

		DCORE_CLASS_PLACEMENT_NEW_DELETE

		void* _sema;
	};

	xsemaphore::xsemaphore(s32 n, s32 max)
	{
		ASSERT(n >= 0 && max > 0 && n <= max);
		xsemaphore_win* data = (xsemaphore_win*)this;
		data->_sema = CreateSemaphoreW(nullptr, n, max, nullptr);
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
		if (!ReleaseSemaphore(data->_sema, 1, nullptr))
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
		fsadexed_array_t m_alloc;
		DCORE_CLASS_PLACEMENT_NEW_DELETE
	};


	xsemaphores_data*	gCreateSemaphoresData(alloc_t* alloc, u32 max_semaphores)
	{
		xsemaphores_data* semas = alloc->construct<xsemaphores_data>();
		xsemaphore_win* sema_array = (xsemaphore_win*)alloc->allocate(sizeof(xsemaphore_win) * max_semaphores);
		semas->m_alloc = fsadexed_array_t(sema_array, sizeof(xsemaphore_win), max_semaphores);
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


} // namespace ncore

#endif