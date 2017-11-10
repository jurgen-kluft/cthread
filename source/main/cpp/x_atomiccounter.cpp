#include "xmthread\x_atomiccounter.h"

#if defined(TARGET_PC)
#include <Windows.h>
#endif

namespace xcore 
{
#if defined(TARGET_PC)
	//
	// Windows
	//
	xatomcnt::xatomcnt()
		: _counter(0)
	{
	}


	xatomcnt::xatomcnt(xatomcnt::xvalue_t initialValue)
		: _counter(initialValue)
	{
	}


	xatomcnt::xatomcnt(const xatomcnt& counter)
		: _counter(counter.value())
	{
	}


	xatomcnt::~xatomcnt()
	{
	}


	xatomcnt& xatomcnt::operator = (const xatomcnt& counter)
	{
		::InterlockedExchange(&_counter, counter.value());
		return *this;
	}


	xatomcnt& xatomcnt::operator = (xatomcnt::xvalue_t value)
	{
		::InterlockedExchange(&_counter, value);
		return *this;
	}

	xatomcnt::xvalue_t xatomcnt::operator ++ () // prefix
	{
		return ::InterlockedIncrement(&_counter);
	}


	xatomcnt::xvalue_t xatomcnt::operator ++ (s32) // postfix
	{
		xvalue_t result = InterlockedIncrement(&_counter);
		return --result;
	}


	xatomcnt::xvalue_t xatomcnt::operator -- () // prefix
	{
		return InterlockedDecrement(&_counter);
	}


	xatomcnt::xvalue_t xatomcnt::operator -- (s32) // postfix
	{
		xvalue_t result = InterlockedDecrement(&_counter);
		return ++result;
	}

#elif 
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


#endif // TARGET_...


} // namespace xcore
