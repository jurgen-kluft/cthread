#include "xmthread\x_event.h"
#include "xmthread\x_synchronizedobject.h"

namespace xcore 
{
	xevent::xevent(bool autoReset): xevent_impl(autoReset)
	{
	}


	xevent::~xevent()
	{
	}


} // namespace xcore
