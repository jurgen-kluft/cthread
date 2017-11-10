#include "xmthread\private\x_namedmutex_win32.h"

namespace xcore 
{
	static void copy_str(char* to, s32 max_len, const char* from)
	{
		for (s32 i=0; i<max_len; ++i)
		{
			to[i] = from[i];
			if (from[i] == '\0')
				break;
		}
	}

	xnmutex_impl::xnmutex_impl(const char* name)
	{
		copy_str(_name, sizeof(_name), name);

		_mutex = CreateMutexA(NULL, FALSE, _name);
		if (!_mutex) 
		{
			// cannot create named mutex
		}
	}


	xnmutex_impl::~xnmutex_impl()
	{
		CloseHandle(_mutex);
	}


	void xnmutex_impl::lockImpl()
	{
		switch (WaitForSingleObject(_mutex, INFINITE))
		{
		case WAIT_OBJECT_0:
			return;
		case WAIT_ABANDONED:
			// cannot lock named mutex (abadoned)
			break;
		default:
			// cannot lock named mutex
			break;
		}
	}


	bool xnmutex_impl::tryLockImpl()
	{
		switch (WaitForSingleObject(_mutex, 0))
		{
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		case WAIT_ABANDONED:
			// cannot lock named mutex (abadoned)
			break;
		default:
			// cannot lock named mutex
			break;
		}
		return false;
	}


	void xnmutex_impl::unlockImpl()
	{
		ReleaseMutex(_mutex);
	}


} // namespace xcore
