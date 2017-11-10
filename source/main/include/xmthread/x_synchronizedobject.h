#ifndef __XMTHREAD_SYNCHRONIZEDOBJECT_H__
#define __XMTHREAD_SYNCHRONIZEDOBJECT_H__
#include "xbase\x_target.h"

#include "xmthread\x_mutex.h"
#include "xmthread\x_event.h"

namespace xcore 
{
	/// This class aggregates a xmutex and an xevent
	/// and can act as a base class for all objects
	/// requiring synchronization in a multithreaded
	/// scenario.
	class xsynced_object
	{
	public:
		typedef xcore::xscoped_lock<xsynced_object> xscoped_lock;

						xsynced_object();
		/// Creates the object.

		virtual			~xsynced_object();
		/// Destroys the object.

		void			lock() const;
		/// Locks the object. Blocks if the object
		/// is locked by another thread.

		bool			tryLock() const;
		/// Tries to lock the object. Returns false immediately
		/// if the object is already locked by another thread
		/// Returns true if the object was successfully locked.

		void			unlock() const;
		/// Unlocks the object so that it can be locked by
		/// other threads.

		void			notify() const;
		/// Signals the object. 
		/// Exactly only one thread waiting for the object 
		/// can resume execution.

		void			wait() const;
		/// Waits for the object to become signalled.

		void			wait(u32 milliseconds) const;
		/// Waits for the object to become signalled.
		/// Throws a TimeoutException if the object
		/// does not become signalled within the specified
		/// time interval.

		bool			tryWait(u32 milliseconds) const;
		/// Waits for the object to become signalled.
		/// Returns true if the object
		/// became signalled within the specified
		/// time interval, false otherwise.

	private:
		mutable xmutex	_mutex;
		mutable xevent	_event;
	};


	//
	// inlines
	//
	inline void xsynced_object::lock() const
	{
		_mutex.lock();
	}

	inline bool xsynced_object::tryLock() const
	{
		return _mutex.tryLock();
	}

	inline void xsynced_object::unlock() const
	{
		_mutex.unlock();
	}

	inline void xsynced_object::notify() const
	{
		_event.set();
	}

	inline void xsynced_object::wait() const
	{
		_event.wait();
	}

	inline void xsynced_object::wait(u32 milliseconds) const
	{
		_event.wait(milliseconds);
	}

	inline bool xsynced_object::tryWait(u32 milliseconds) const
	{
		return _event.tryWait(milliseconds);
	}

} // namespace xcore

#endif // __XMTHREAD_SYNCHRONIZEDOBJECT_H__
