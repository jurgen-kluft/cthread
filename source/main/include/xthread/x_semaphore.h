#ifndef __XMTHREAD_SEMAPHORE_H__
#define __XMTHREAD_SEMAPHORE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	
	// A xsemaphore is a synchronization object with the following 
	// characteristics:
	// A semaphore has a value that is constrained to be a non-negative
	// integer and two atomic operations. The allowable operations are V 
	// (here called set()) and P (here called wait()). A V (set()) operation 
	// increases the value of the semaphore by one. 
	// A P (wait()) operation decreases the value of the semaphore by one, 
	// provided that can be done without violating the constraint that the 
	// value be non-negative. A P (wait()) operation that is initiated when 
	// the value of the semaphore is 0 suspends the calling thread. 
	// The calling thread may continue when the value becomes positive again.
	class xsemaphore
	{
	public:
					xsemaphore(s32 n);
					xsemaphore(s32 n, s32 max);
		// Creates the semaphore. The current value
		// of the semaphore is given in n. The
		// maximum value of the semaphore is given
		// in max.
		// If only n is given, it must be greater than
		// zero.
		// If both n and max are given, max must be
		// greater than zero, n must be greater than
		// or equal to zero and less than or equal
		// to max.

					~xsemaphore();
		// Destroys the semaphore.

		void		signal();
		// Increments the semaphore's value by one and
		// thus signals the semaphore. Another thread
		// waiting for the semaphore will be able
		// to continue.

		void		wait();
		// Waits for the semaphore to become signalled.
		// To become signalled, a semaphore's value must
		// be greater than zero. 
		// Decrements the semaphore's value by one.

		bool		try_wait(u32 milliseconds);
		// Waits for the semaphore to become signalled.
		// To become signalled, a semaphore's value must
		// be greater than zero. 
		// Decrements the semaphore's value by one.
		// Returns false if there is a time-out and true
		// if we got a signal.

	private:
					xsemaphore();
					xsemaphore(const xsemaphore&);

		xsemaphore& operator = (const xsemaphore&) {}
	};

} // namespace ncore


#endif // __XMTHREAD_SEMAPHORE_H__
