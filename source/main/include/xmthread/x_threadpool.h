#ifndef __XMTHREAD_THREADPOOL_H__
#define __XMTHREAD_THREADPOOL_H__
#include "xbase\x_target.h"

#include "xmthread\x_thread.h"
#include "xmthread\x_mutex.h"

namespace xcore
{
	class x_iallocator;
	class xrunnable;
	class xpoolthread;
	struct xthreadvec;

	/// A thread pool always keeps a number of threads running, ready
	/// to accept work.
	/// Creating and starting a threads can impose a significant runtime
	/// overhead to an application. A thread pool helps to improve
	/// the performance of an application by reducing the number
	/// of threads that have to be created (and destroyed again).
	/// Threads in a thread pool are re-used once they become
	/// available again.
	/// The thread pool always keeps a minimum number of threads
	/// running. If the demans for threads increases, additional
	/// threads are created. Once the demand for threads sinks
	/// again, no-longer used threads are stopped and removed
	/// from the pool.
	class xthreadpool
	{
	public:
		xthreadpool(x_iallocator* allocator, u32 minCapacity = 2, u32 maxCapacity = 16, u32 idleTime = 60, u32 stackSize = XMTHREAD_STACK_SIZE);
		/// Creates a thread pool with minCapacity threads.
		/// If required, up to maxCapacity threads are created
		/// a NoThreadAvailableException exception is thrown.
		/// If a thread is running idle for more than idleTime seconds,
		/// and more than minCapacity threads are running, the thread
		/// is killed. Threads are created with given stack size.

		xthreadpool(x_iallocator* allocator, const char* name, u32 minCapacity = 2, u32 maxCapacity = 16, u32 idleTime = 60, u32 stackSize = XMTHREAD_STACK_SIZE);
		/// Creates a thread pool with the given name and minCapacity threads.
		/// If required, up to maxCapacity threads are created
		/// a NoThreadAvailableException exception is thrown.
		/// If a thread is running idle for more than idleTime seconds,
		/// and more than minCapacity threads are running, the thread
		/// is killed. Threads are created with given stack size.

		~xthreadpool();
		/// Currently running threads will remain active
		/// until they complete. 

		u32			capacity() const;
		/// Returns the maximum capacity of threads.

		void		setStackSize(u32 stackSize);
		/// Sets the stack size for threads.
		/// New stack size applies only for newly created threads.

		u32			getStackSize() const;
		/// Returns the stack size used to create new threads.

		u32			used() const;
		/// Returns the number of currently used threads.

		u32			allocated() const;
		/// Returns the number of currently allocated threads.

		u32			available() const;
		/// Returns the number available threads.

		bool		start(xrunnable& target);
		/// Obtains a thread and starts the target.
		/// Throws a NoThreadAvailableException if no more
		/// threads are available.

		bool		start(xrunnable& target, const char* name);
		/// Obtains a thread and starts the target.
		/// Assigns the given name to the thread.
		/// Throws a NoThreadAvailableException if no more
		/// threads are available.

		bool		startWithPriority(xthread::Priority priority, xrunnable& target);
		/// Obtains a thread, adjusts the thread's priority, and starts the target.
		/// Throws a NoThreadAvailableException if no more
		/// threads are available.

		bool		startWithPriority(xthread::Priority priority, xrunnable& target, const char* name);
		/// Obtains a thread, adjusts the thread's priority, and starts the target.
		/// Assigns the given name to the thread.
		/// Throws a NoThreadAvailableException if no more
		/// threads are available.

		void		stopAll();
		/// Stops all running threads and waits for their completion.
		///
		/// Will also delete all thread objects.
		/// If used, this method should be the last action before
		/// the thread pool is deleted.
		///
		/// Note: If a thread fails to stop within 10 seconds 
		/// (due to a programming error, for example), the
		/// underlying thread object will not be deleted and
		/// this method will return anyway. This allows for a
		/// more or less graceful shutdown in case of a misbehaving
		/// thread.

		void		joinAll();
		/// Waits for all threads to complete.
		///
		/// Note that this will not actually join() the underlying
		/// thread, but rather wait for the thread's runnables
		/// to finish.

		void		collect();
		/// Stops and removes no longer used threads from the
		/// thread pool. Can be called at various times in an
		/// application's life time to help the thread pool
		/// manage its threads. Calling this method is optional,
		/// as the thread pool is also implicitly managed in
		/// calls to start(), addCapacity() and joinAll().

		const char* name() const;
		/// Returns the name of the thread pool,
		/// or an empty string if no name has been
		/// specified in the constructor.

	protected:
		xpoolthread*		getThread();
		xpoolthread*		createThread();

		void				housekeep();

	private:
							xthreadpool(const xthreadpool& pool);
							xthreadpool& operator = (const xthreadpool& pool);

		x_iallocator*		_allocator;
		char				_name[64];
		u32					_minCapacity;
		u32					_maxCapacity;
		u64					_idleTime;
		u32					_serial;
		u32					_age;
		u32					_stackSize;
		xthreadvec*			_threads;
		mutable xfastmutex	_mutex;
	};


	//
	// inlines
	//
	inline void xthreadpool::setStackSize(u32 stackSize)
	{
		_stackSize = stackSize;
	}


	inline u32 xthreadpool::getStackSize() const
	{
		return _stackSize;
	}


	inline const char* xthreadpool::name() const
	{
		return _name;
	}


} // namespace xcore


#endif // __XMTHREAD_THREADPOOL_H__
