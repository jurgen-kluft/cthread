#ifndef __XMTHREAD_THREAD_H__
#define __XMTHREAD_THREAD_H__
#include "xbase\x_target.h"

#include "xmthread\x_mutex.h"
#include "xmthread\x_threadlocal.h"

#if defined(TARGET_PC)
#include "xmthread\private\x_thread_win32.h"
#endif

namespace xcore
{
	class xrunnable;
	class xthread_ls;

	/// This class implements a platform-independent
	/// wrapper to an operating system thread.
	///
	/// Every xthread object gets a unique (within
	/// its process) numeric thread ID.
	/// Furthermore, a thread can be assigned a name.
	/// The name of a thread can be changed at any time.
	class xthread : private xthread_impl
	{
	public:	
		typedef xthread_impl::TIDImpl TID;

		using xthread_impl::Callable;

		enum Priority
			/// xthread priorities.
		{
			PRIO_LOWEST  = PRIO_LOWEST_IMPL, /// The lowest thread priority.
			PRIO_LOW     = PRIO_LOW_IMPL,    /// A lower than normal thread priority.
			PRIO_NORMAL  = PRIO_NORMAL_IMPL, /// The normal thread priority.
			PRIO_HIGH    = PRIO_HIGH_IMPL,   /// A higher than normal thread priority.
			PRIO_HIGHEST = PRIO_HIGHEST_IMPL /// The highest thread priority.
		};

							xthread();
		/// Creates a thread. Call start() to start it.

							xthread(const char* name);
		/// Creates a named thread. Call start() to start it.

							~xthread();
		/// Destroys the thread.

		s32					id() const;
		/// Returns the unique thread ID of the thread.

		TID					tid() const;
		/// Returns the native thread ID of the thread.

		const char*			name() const;
		/// Returns the name of the thread.

		const char*			getName() const;
		/// Returns the name of the thread.

		void				setName(const char* name);
		/// Sets the name of the thread.

		void				setPriority(Priority prio);
		/// Sets the thread's priority.
		///
		/// Some platform only allow changing a thread's priority
		/// if the process has certain privileges.

		Priority			getPriority() const;
		/// Returns the thread's priority.

		void				setOSPriority(s32 prio);
		/// Sets the thread's priority, using an operating system specific
		/// priority value. Use getMinOSPriority() and getMaxOSPriority() to
		/// obtain mininum and maximum priority values.

		s32					getOSPriority() const;
		/// Returns the thread's priority, expressed as an operating system
		/// specific priority value.
		///
		/// May return 0 if the priority has not been explicitly set.

		static s32			getMinOSPriority();
		/// Returns the mininum operating system-specific priority value,
		/// which can be passed to setOSPriority().

		static s32			getMaxOSPriority();
		/// Returns the maximum operating system-specific priority value,
		/// which can be passed to setOSPriority().

		void				setStackSize(s32 size);
		/// Sets the thread's stack size in bytes.
		/// Setting the stack size to 0 will use the default stack size.
		/// Typically, the real stack size is rounded up to the nearest
		/// page size multiple.

		s32					getStackSize() const;
		/// Returns the thread's stack size in bytes.
		/// If the default stack size is used, 0 is returned.

		void				start(xrunnable& target);
		/// Starts the thread with the given target.

		void				start(Callable target, void* pData = 0);
		/// Starts the thread with the given target and parameter.

		void				join();
		/// Waits until the thread completes execution.	
		/// If multiple threads try to join the same
		/// thread, the result is undefined.

		void				join(u32 milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Throws a TimeoutException if the thread
		/// does not complete within the specified time interval.

		bool				tryJoin(u32 milliseconds);
		/// Waits for at most the given interval for the thread
		/// to complete. Returns true if the thread has finished,
		/// false otherwise.

		bool				isRunning() const;
		/// Returns true if the thread is running.

		static void			sleep(u32 milliseconds);
		/// Suspends the current thread for the specified
		/// amount of time.

		static void			yield();
		/// Yields cpu to other threads.

		static xthread*		current();
		/// Returns the xthread object for the currently active thread.
		/// If the current thread is the main thread, 0 is returned.

		static TID			currentTid();
		/// Returns the native thread ID for the current thread.    

	protected:
		xthread_ls*			tls();
		/// Returns a reference to the thread's local storage.

		void				makeName(char* str, s32 max_len);
		/// Creates a unique name for a thread.

		static s32			uniqueId();
		/// Creates and returns a unique id for a thread.

	private:
							xthread(const xthread&);
		xthread&			operator = (const xthread&);

		s32					_id;
		char				_name[64];
		xthread_ls			_pTLS;
		mutable xfastmutex	_mutex;

		friend class xthread_ls;
		friend class xpoolthread;
	};


	//
	// inlines
	//
	inline xthread::TID xthread::tid() const
	{
		return tidImpl();
	}


	inline s32 xthread::id() const
	{
		return _id;
	}


	inline const char* xthread::name() const
	{
		xfastmutex::xscoped_lock lock(_mutex);
		return _name;
	}


	inline const char* xthread::getName() const
	{
		xfastmutex::xscoped_lock lock(_mutex);
		return _name;
	}


	inline bool xthread::isRunning() const
	{
		return isRunningImpl();
	}


	inline void xthread::sleep(u32 milliseconds)
	{
		sleepImpl(milliseconds);
	}


	inline void xthread::yield()
	{
		yieldImpl();
	}


	inline xthread* xthread::current()
	{
		return static_cast<xthread*>(currentImpl());
	}


	inline void xthread::setOSPriority(s32 prio)
	{
		setOSPriorityImpl(prio);	
	}


	inline s32 xthread::getOSPriority() const
	{
		return getOSPriorityImpl();
	}


	inline s32 xthread::getMinOSPriority()
	{
		return xthread_impl::getMinOSPriorityImpl();
	}


	inline s32 xthread::getMaxOSPriority()
	{
		return xthread_impl::getMaxOSPriorityImpl();
	}


	inline void xthread::setStackSize(s32 size)
	{
		setStackSizeImpl(size);
	}


	inline s32 xthread::getStackSize() const
	{
		return getStackSizeImpl();
	}


	inline xthread::TID xthread::currentTid()
	{
		return currentTidImpl();
	}


} // namespace xcore


#endif // __XMTHREAD_THREAD_H__
