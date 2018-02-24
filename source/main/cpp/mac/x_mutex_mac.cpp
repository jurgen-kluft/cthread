#include "xtime/x_datetime.h"

#ifdef TARGET_MAC
#include "xthread/private/osx/x_mutex_mac.h"

namespace xcore
{
	xmutex_impl::xmutex_impl()
	{
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);

		pthread_mutex_init(&_mutex, &attr)

		pthread_mutexattr_destroy(&attr);
	}

	xmutex_impl::~xmutex_impl()
	{
		pthread_mutex_destroy(&_mutex);
	}

} // namespace xcore

#endif