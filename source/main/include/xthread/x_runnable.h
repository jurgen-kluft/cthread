#ifndef __XMTHREAD_RUNNABLE_H__
#define __XMTHREAD_RUNNABLE_H__
#include "xbase\x_target.h"

namespace xcore
{
	/// The xrunnable interface with the run() method
	/// must be implemented by classes that provide
	/// an entry point for a thread.
	class xrunnable
	{
	public:	
						xrunnable();
		virtual			~xrunnable();

		virtual void	run() = 0;
		/// Do whatever the thread needs to do. Must
		/// be overridden by subclasses.
	};


} // namespace xcore


#endif // __XMTHREAD_RUNNABLE_H__
