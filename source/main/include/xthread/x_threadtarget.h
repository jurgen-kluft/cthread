#ifndef __XMTHREAD_THREADTARGET_H__
#define __XMTHREAD_THREADTARGET_H__
#include "xbase\x_target.h"

#include "xmthread\x_runnable.h"

namespace xcore
{
	/// This adapter simplifies using static member functions as well as 
	/// standalone functions as targets for threads.
	/// Note that it is possible to pass those entities directly to xthread::start().
	/// This adapter is provided as a convenience for higher abstraction level
	/// scenarios where xrunnable abstract class is used.
	///
	/// For using a non-static member function as a thread target, please
	/// see the xrunnable_adapter class.
	/// 
	/// Usage:
	///    class MyObject
	///    {
	///        static void doSomething() {}
	///    };
	///    xthread_target ra(&MyObject::doSomething);
	///    xthread thr;
	///    thr.start(ra);
	///
	/// or:
	/// 
	///    void doSomething() {}
	/// 
	///    xthread_target ra(doSomething);
	///    xthread thr;
	///    thr.start(ra);
	class xthread_target : public xrunnable
	{
	public:
		typedef void (*callback)();

						xthread_target(callback method);
						xthread_target(const xthread_target& te);
						~xthread_target();

		xthread_target&	operator = (const xthread_target& te);

		void			run();

	private:
						xthread_target();
		callback		_method;
	};

	//
	// inlines
	//
	inline void xthread_target::run()
	{
		_method();
	}


} // namespace xcore


#endif // __XMTHREAD_THREADTARGET_H__
