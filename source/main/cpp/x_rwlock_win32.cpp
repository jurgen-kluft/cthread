#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xmthread\private\x_rwlock_win32.h"

namespace xcore 
{
	xrwlock_impl::xrwlock_impl()
		: _readers(0)
		, _writersWaiting(0)
		, _writers(0)
	{
		_mutex = CreateMutexW(NULL, FALSE, NULL);
		if (_mutex == NULL)
		{
			// cannot create reader/writer lock
		}
		else
		{
			_readEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
			if (_readEvent == NULL)
			{
				// cannot create reader/writer lock
			}
			else
			{
				_writeEvent = CreateEventW(NULL, TRUE, TRUE, NULL);
				if (_writeEvent == NULL)
				{
					// cannot create reader/writer lock
				}
			}
		}
	}

	xrwlock_impl::~xrwlock_impl()
	{
		CloseHandle(_mutex);
		CloseHandle(_readEvent);
		CloseHandle(_writeEvent);
	}

	inline void xrwlock_impl::addWriter()
	{
		switch (WaitForSingleObject(_mutex, INFINITE))
		{
		case WAIT_OBJECT_0:
			if (++_writersWaiting == 1) 
				ResetEvent(_readEvent);
			ReleaseMutex(_mutex);
			break;
		default:
			// cannot lock reader/writer lock
			break;
		}
	}


	inline void xrwlock_impl::removeWriter()
	{
		switch (WaitForSingleObject(_mutex, INFINITE))
		{
		case WAIT_OBJECT_0:
			if (--_writersWaiting == 0 && _writers == 0) 
				SetEvent(_readEvent);
			ReleaseMutex(_mutex);
			break;
		default:
			// cannot lock reader/writer lock
			break;
		}
	}


	void xrwlock_impl::readLockImpl()
	{
		HANDLE h[2];
		h[0] = _mutex;
		h[1] = _readEvent;
		switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			++_readers;
			ResetEvent(_writeEvent);
			ReleaseMutex(_mutex);
			ASSERT(_writers == 0);
			break;
		default:
			// cannot lock reader/writer lock
			break;
		}
	}


	bool xrwlock_impl::tryReadLockImpl()
	{
		for (;;)
		{
			if (_writers != 0 || _writersWaiting != 0)
				return false;

			DWORD result = tryReadLockOnce();
			switch (result)
			{
			case WAIT_OBJECT_0:
			case WAIT_OBJECT_0 + 1:
				return true;
			case WAIT_TIMEOUT:
				continue; // try again
			default:
				// cannot lock reader/writer lock
				break;
			}
		}
	}


	void xrwlock_impl::writeLockImpl()
	{
		addWriter();
		HANDLE h[2];
		h[0] = _mutex;
		h[1] = _writeEvent;
		switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			--_writersWaiting;
			++_readers;
			++_writers;
			ResetEvent(_readEvent);
			ResetEvent(_writeEvent);
			ReleaseMutex(_mutex);
			ASSERT(_writers == 1);
			break;
		default:
			removeWriter();
			// cannot lock reader/writer lock
			break;
		}
	}


	bool xrwlock_impl::tryWriteLockImpl()
	{
		addWriter();
		HANDLE h[2];
		h[0] = _mutex;
		h[1] = _writeEvent;
		switch (WaitForMultipleObjects(2, h, TRUE, 1))
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			--_writersWaiting;
			++_readers;
			++_writers;
			ResetEvent(_readEvent);
			ResetEvent(_writeEvent);
			ReleaseMutex(_mutex);
			ASSERT(_writers == 1);
			return true;
		case WAIT_TIMEOUT:
			removeWriter();
			return false;
		default:
			removeWriter();
			// cannot lock reader/writer lock
			break;
		}
		return false;
	}


	void xrwlock_impl::unlockImpl()
	{
		switch (WaitForSingleObject(_mutex, INFINITE))
		{
		case WAIT_OBJECT_0:
			_writers = 0;
			if (_writersWaiting == 0) 
				SetEvent(_readEvent);
			if (--_readers == 0)
				SetEvent(_writeEvent);
			ReleaseMutex(_mutex);
			break;
		default:
			// cannot lock reader/writer lock
			break;
		}
	}

	DWORD xrwlock_impl::tryReadLockOnce()
	{
		HANDLE h[2];
		h[0] = _mutex;
		h[1] = _readEvent;
		DWORD result = WaitForMultipleObjects(2, h, TRUE, 1); 
		switch (result)
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			++_readers;
			ResetEvent(_writeEvent);
			ReleaseMutex(_mutex);
			ASSERT(_writers == 0);
			return result;
		case WAIT_TIMEOUT:
			return result;
		default:
			// cannot lock reader/writer lock
			return -5;
		}
	}


} // namespace xcore
