#include "xthread/x_atomiccounter.h"

#if defined(TARGET_OSX)
#include <atomic>
#endif

namespace xcore 
{
#if defined(TARGET_OSX)
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
		_counter.store(counter._counter);
		return *this;
	}


	xatomcnt& xatomcnt::operator = (xatomcnt::xvalue_t value)
	{
		_counter.store(value);
		return *this;
	}

	xatomcnt::xvalue_t xatomcnt::operator ++ () // prefix
	{
		xvalue_t old = ++_counter;
		return old;
	}


	xatomcnt::xvalue_t xatomcnt::operator ++ (s32) // postfix
	{
		xvalue_t old = _counter++;
		return old;
	}


	xatomcnt::xvalue_t xatomcnt::operator -- () // prefix
	{
		xvalue_t old = --_counter;
		return old;
	}


	xatomcnt::xvalue_t xatomcnt::operator -- (s32) // postfix
	{
		xvalue_t old = _counter--;
		return old;
	}

#endif


} // namespace xcore
