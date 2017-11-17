#include "xthread/x_atomiccounter.h"

#ifdef XTHREAD_ATOMIC_COUNTER_PLACEHOLDER

namespace xcore
{
	//
	// Generic implementation based on xfastmutex
	//
	xatomcnt::xatomcnt()
	{
		_counter.value = 0;
	}


	xatomcnt::xatomcnt(xatomcnt::xvalue_t initialValue)
	{
		_counter.value = initialValue;
	}


	xatomcnt::xatomcnt(const xatomcnt& counter)
	{
		_counter.value = counter.value();
	}


	xatomcnt::~xatomcnt()
	{
	}


	xatomcnt& xatomcnt::operator = (const xatomcnt& counter)
	{
		xfastmutex::xscoped_lock lock(_counter.mutex);
		_counter.value = counter.value();
		return *this;
	}


	xatomcnt& xatomcnt::operator = (xatomcnt::xvalue_t value)
	{
		xfastmutex::xscoped_lock lock(_counter.mutex);
		_counter.value = value;
		return *this;
	}
} // namespace xcore

#endif // XTHREAD_ATOMIC_COUNTER_USE_MUTEX
