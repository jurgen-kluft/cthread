#ifndef __XMTHREAD_ATOMICCOUNTER_H__
#define __XMTHREAD_ATOMICCOUNTER_H__

#include "xbase/x_target.h"

namespace xcore 
{
	// This class implements a simple counter, which
	// provides atomic operations that are safe to
	// use in a multithreaded environment.
	//
	// Typical usage of xatomcnt is for implementing
	// reference counting and similar things.
	//
	// On some platforms, the implementation of xatomcnt
	// is based on atomic primitives specific to the platform
	// (such as InterlockedIncrement, etc. on Windows), and
	// thus very efficient. On platforms that do not support
	// atomic primitives, operations are guarded by a xfastmutex.
	//
	// The following platforms currently have atomic primitives:
	//   - Windows
	//   - PS3
	//   - Xbox 360
	//   - PS4
	//   - Xbox One
	class xatomcnt
	{
	public:
		typedef s32		xvalue_t; /// The underlying integer type.

						xatomcnt();
		/// Creates a new xatomcnt and initializes it to zero.

		explicit		xatomcnt(xvalue_t initialValue);
		/// Creates a new xatomcnt and initializes it with
		/// the given value.

						xatomcnt(const xatomcnt& counter);
		/// Creates the counter by copying another one.

						~xatomcnt();
		/// Destroys the xatomcnt.

		xatomcnt&		operator = (const xatomcnt& counter);
		/// Assigns the value of another xatomcnt.

		xatomcnt&		operator = (xvalue_t value);
		/// Assigns a value to the counter.

						operator xvalue_t () const;
		/// Returns the value of the counter.

		xvalue_t		value() const;
		/// Returns the value of the counter.

		xvalue_t		operator ++ (); // prefix
		/// Increments the counter and returns the result.

		xvalue_t		operator ++ (s32); // postfix
		/// Increments the counter and returns the previous value.

		xvalue_t		operator -- (); // prefix
		/// Decrements the counter and returns the result.

		xvalue_t		operator -- (s32); // postfix
		/// Decrements the counter and returns the previous value.

		bool			operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

	private:
#if defined(TARGET_PC)
		volatile u32	_counter;
#endif // TARGET_PC
	};


	//
	// inlines
	//


#if defined(TARGET_PC)
	//
	// Windows
	//
	inline xatomcnt::operator xatomcnt::xvalue_t () const
	{
		return _counter;
	}


	inline xatomcnt::xvalue_t xatomcnt::value() const
	{
		return _counter;
	}


	inline bool xatomcnt::operator ! () const
	{
		return _counter == 0;
	}


#else
	//
	// Generic implementation 
	//
	inline xatomcnt::operator xatomcnt::xvalue_t () const
	{
		return _counter.value;
	}


	inline xatomcnt::xvalue_t xatomcnt::value() const
	{
		return _counter.value;
	}


	inline xatomcnt::xvalue_t xatomcnt::operator ++ () // prefix
	{
		--_counter.value;
		return _counter.value;
	}


	inline xatomcnt::xvalue_t xatomcnt::operator ++ (s32) // postfix
	{
		xvalue_t result = _counter.value;
		_counter.value += 1;
		return result;
	}


	inline xatomcnt::xvalue_t xatomcnt::operator -- () // prefix
	{
		--_counter.value;
		xvalue_t result = _counter.value;
		return result;
	}


	inline xatomcnt::xvalue_t xatomcnt::operator -- (s32) // postfix
	{
		xvalue_t result = _counter.value--;
		return result;
	}

	inline bool xatomcnt::operator ! () const
	{
		bool result = _counter.value == 0;
		return result;
	}


#endif // TARGET_...


} // namespace xcore


#endif // __XMTHREAD_ATOMICCOUNTER_H__
