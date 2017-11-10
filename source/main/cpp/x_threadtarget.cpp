#include "xmthread\x_threadtarget.h"

namespace xcore 
{
	xthread_target::xthread_target(callback method)
		: _method(method)
	{
	}


	xthread_target::xthread_target(const xthread_target& te)
		: _method(te._method)
	{
	}


	xthread_target& xthread_target::operator = (const xthread_target& te)
	{
		_method  = te._method;
		return *this;
	}


	xthread_target::~xthread_target()
	{
	}


} // namespace xcore
