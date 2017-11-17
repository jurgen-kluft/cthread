#include "xthread/private/osx/x_mutex_osx.h"
#include "xtime/x_datetime.h"

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
