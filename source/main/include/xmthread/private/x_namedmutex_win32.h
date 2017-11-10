#ifndef __XMTHREAD_NAMEDMUTEX_WIN32_H__
#define __XMTHREAD_NAMEDMUTEX_WIN32_H__

#include "xbase\x_target.h"
#include <Windows.h>

namespace xcore 
{
	class xnmutex_impl
	{
	protected:
					xnmutex_impl(const char* name);
					~xnmutex_impl();

		void		lockImpl();
		bool		tryLockImpl();
		void		unlockImpl();

	private:
		char		_name[64 - sizeof(HANDLE)];
		HANDLE      _mutex;
	};


} // namespace xcore


#endif // __XMTHREAD_NAMEDMUTEX_WIN32_H__
