#ifndef __XMTHREAD_FUNCTOR_H__
#define __XMTHREAD_FUNCTOR_H___
#include "xbase/x_target.h"

namespace xcore
{
	class xthread;

	/// The xthread functor interface with the run() method
	/// must be implemented by derived classes to provide
	/// an entry point for a thread.
	class xthread_functor
	{
	public:	
		virtual			~xthread_functor() {}
		virtual void	run() = 0;
	};


} // namespace xcore


#endif // __XMTHREAD_FUNCTOR_H__