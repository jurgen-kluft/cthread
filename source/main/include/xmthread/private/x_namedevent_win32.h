#ifndef __XMTHREAD_NAMEDEVENT_WIN32_H__
#define __XMTHREAD_NAMEDEVENT_WIN32_H__

#include "xbase\x_target.h"
#include <Windows.h>

namespace xcore
{
	class xnevent_impl
	{
	protected:
					xnevent_impl(const char* name);	
					~xnevent_impl();

		void		setImpl();
		void		waitImpl();

	private:
		char		_name[64 - sizeof(HANDLE)];
		HANDLE      _event;
	};


} // namespace xcore


#endif // __XMTHREAD_NAMEDEVENT_WIN32_H__
