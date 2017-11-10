#include "xmthread\x_namedmutex.h"


namespace xcore 
{
	xnmutex::xnmutex(const char* name)
		: xnmutex_impl(name)
	{
	}


	xnmutex::~xnmutex()
	{
	}


} // namespace xcore
