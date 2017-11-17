#include "xthread/x_event.h"

namespace xcore 
{
	xevent::xevent(bool autoReset): xevent_impl(autoReset)
	{
	}

	xevent::~xevent()
	{
	}

} // namespace xcore
