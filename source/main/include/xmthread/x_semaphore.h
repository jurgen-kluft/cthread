#ifndef __XMTHREAD_SEMAPHORE_H__
#define __XMTHREAD_SEMAPHORE_H__
#include "xbase\x_target.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_semaphore_win32.h"
#endif

namespace xcore
{
	/// A xsemaphore is a synchronization object with the following 
	/// characteristics:
	/// A semaphore has a value that is constrained to be a non-negative
	/// integer and two atomic operations. The allowable operations are V 
	/// (here called set()) and P (here called wait()). A V (set()) operation 
	/// increases the value of the semaphore by one. 
	/// A P (wait()) operation decreases the value of the semaphore by one, 
	/// provided that can be done without violating the constraint that the 
	/// value be non-negative. A P (wait()) operation that is initiated when 
	/// the value of the semaphore is 0 suspends the calling thread. 
	/// The calling thread may continue when the value becomes positive again.
	class xsemaphore : private xsemaphore_impl
	{
	public:
					xsemaphore(s32 n);
					xsemaphore(s32 n, s32 max);
		/// Creates the semaphore. The current value
		/// of the semaphore is given in n. The
		/// maximum value of the semaphore is given
		/// in max.
		/// If only n is given, it must be greater than
		/// zero.
		/// If both n and max are given, max must be
		/// greater than zero, n must be greater than
		/// or equal to zero and less than or equal
		/// to max.

					~xsemaphore();
		/// Destroys the semaphore.

		void		set();
		/// Increments the semaphore's value by one and
		/// thus signals the semaphore. Another thread
		/// waiting for the semaphore will be able
		/// to continue.

		void		wait();
		/// Waits for the semaphore to become signalled.
		/// To become signalled, a semaphore's value must
		/// be greater than zero. 
		/// Decrements the semaphore's value by one.

		void		wait(u32 milliseconds);
		/// Waits for the semaphore to become signalled.
		/// To become signalled, a semaphore's value must
		/// be greater than zero.
		/// Throws a TimeoutException if the semaphore
		/// does not become signalled within the specified
		/// time interval.
		/// Decrements the semaphore's value by one
		/// if successful.

		bool		tryWait(u32 milliseconds);
		/// Waits for the semaphore to become signalled.
		/// To become signalled, a semaphore's value must
		/// be greater than zero.
		/// Returns true if the semaphore
		/// became signalled within the specified
		/// time interval, false otherwise.
		/// Decrements the semaphore's value by one
		/// if successful.

	private:
					xsemaphore();
					xsemaphore(const xsemaphore&);

		xsemaphore& operator = (const xsemaphore&);
	};


	//
	// inlines
	//
	inline void xsemaphore::set()
	{
		setImpl();
	}


	inline void xsemaphore::wait()
	{
		waitImpl();
	}


	inline void xsemaphore::wait(u32 milliseconds)
	{
		if (!waitImpl(milliseconds))
		{
			// timeout
		}
	}


	inline bool xsemaphore::tryWait(u32 milliseconds)
	{
		return waitImpl(milliseconds);
	}


} // namespace xcore


#endif // __XMTHREAD_SEMAPHORE_H__
