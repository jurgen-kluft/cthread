#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xtime/x_datetime.h"

#ifdef TARGET_PC
#include "xthread/x_threading.h"
#include "xthread/x_mutex.h"
#include <Windows.h>

namespace xcore
{
	class xmutex_win : public xmutex
	{
	public:
		xmutex_win() : xmutex() {}

		XCORE_CLASS_PLACEMENT_NEW_DELETE

		CRITICAL_SECTION _cs;
	};

	xmutex::xmutex()
	{
		xmutex_win* data = (xmutex_win*)this;

		// the func has a boolean return value under WInnNt/2000/XP but not on Win98
		// the return only checks if the input address of &_cs was valid, so it is 
		// safe to omit it.
		InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION*)&data->_cs, 4000);
	}

	xmutex::~xmutex()
	{
		xmutex_win* data = (xmutex_win*)this;
		DeleteCriticalSection((CRITICAL_SECTION*)&data->_cs);
	}

	void xmutex::lock()
	{
		xmutex_win* data = (xmutex_win*)this;
		EnterCriticalSection((CRITICAL_SECTION*)&data->_cs);
	}


	bool xmutex::tryLock()
	{
		xmutex_win* data = (xmutex_win*)this;
		return TryEnterCriticalSection((CRITICAL_SECTION*)&data->_cs) != 0;
	}


	void xmutex::unlock()
	{
		xmutex_win* data = (xmutex_win*)this;
		LeaveCriticalSection((CRITICAL_SECTION*)&data->_cs);
	}


	class xmutexes_data
	{
	public:
		xmutex*         m_events;
		xfsadexed_array m_alloc;
		XCORE_CLASS_PLACEMENT_NEW_DELETE
	};

	xmutexes_data*	gCreateMutexesData(xalloc* alloc, u32 max_mutexes)
	{
		xmutexes_data* events = alloc->construct<xmutexes_data>();
		xmutex_win* event_array = (xmutex_win*)alloc->allocate(sizeof(xmutex_win) * max_mutexes);
		events->m_alloc = xfsadexed_array(event_array, sizeof(xmutex_win), max_mutexes);
		return events;
	}

	xmutex* xthreading::create_mutex()
	{
		xmutex_win* mtx = m_mutexes->m_alloc.construct<xmutex_win>();
		return mtx;
	}

	void xthreading::destroy_mutex(xmutex* mtx)
	{
		xmutex_win* win_mtx = (xmutex_win*)mtx;
		m_mutexes->m_alloc.destruct(win_mtx);
	}

} // namespace xcore

#endif