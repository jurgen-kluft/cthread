#include "ctime/c_datetime.h"

#ifdef TARGET_MAC
#include "cthread/c_mutex.h"

namespace ncore
{
	struct xevent_data
	{
		pthread_mutex_t		_mutex;
	};

	xmutex::xmutex()
	{
		xmutex_data* data = (xmutex_data*)m_data;

		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);

		pthread_mutex_init(&data->_mutex, &attr)

		pthread_mutexattr_destroy(&attr);
	}

	xmutex::~xmutex()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		pthread_mutex_destroy(&data->_mutex);
	}

	void xmutex_impl::lock()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		pthread_mutex_lock(&data->_mutex);
	}


	bool xmutex_impl::tryLock()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		return pthread_mutex_trylock(&data->_mutex) == 0;
	}


	void xmutex_impl::unlock()
	{
		xmutex_data* data = (xmutex_data*)m_data;
		pthread_mutex_unlock(&data->_mutex);
	}
} // namespace ncore

#endif