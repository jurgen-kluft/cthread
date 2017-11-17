#ifndef __XMTHREAD_MUTEX_OSX_H__
#define __XMTHREAD_MUTEX_OSX_H__
#include "xbase/x_target.h"


namespace xcore 
{
	class xmutex_impl
	{
	protected:
							xmutex_impl();
							~xmutex_impl();

		void				mutex_lock();
		bool				mutex_tryLock();
		void				mutex_unlock();

	private:
		pthread_mutex_t		_mutex;
	};

	//
	// inlines
	//
	inline void xmutex_impl::mutex_lock()
	{
		pthread_mutex_lock(&_mutex);
	}


	inline bool xmutex_impl::mutex_tryLock()
	{
		return pthread_mutex_trylock(&_mutex) == 0;
	}


	inline void xmutex_impl::mutex_unlock()
	{
		pthread_mutex_unlock(&_mutex);
	}

} // namespace xcore


#endif // __XMTHREAD_MUTEX_OSX_H__
