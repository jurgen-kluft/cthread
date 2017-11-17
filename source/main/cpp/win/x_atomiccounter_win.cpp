#include "xthread/x_atomiccounter.h"

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

#endif


} // namespace xcore
