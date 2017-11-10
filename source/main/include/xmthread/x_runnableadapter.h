#ifndef __XMTHREAD_RUNNABLEADAPTER_H__
#define __XMTHREAD_RUNNABLEADAPTER_H__
#include "xbase\x_target.h"

#include "xmthread\x_runnable.h"

namespace xcore 
{
	/// This adapter simplifies using ordinary methods as
	/// targets for threads.
	/// Usage:
	///    xrunnable_adapter<MyClass> ra(myObject, &MyObject::doSomething));
	///    xthread thr;
	///    thr.Start(ra);
	///
	/// For using a freestanding or static member function as a thread
	/// target, please see the xthread_target class.
	template <class C>
	class xrunnable_adapter : public xrunnable
	{
	public:
		typedef void (C::*callback)();

		xrunnable_adapter(C& object, callback method)
			: _pObject(&object)
			, _method(method)
		{
		}

		xrunnable_adapter(const xrunnable_adapter& ra)
			: _pObject(ra._pObject)
			, _method(ra._method)
		{
		}

		~xrunnable_adapter()
		{
		}

		xrunnable_adapter& operator = (const xrunnable_adapter& ra)
		{
			_pObject = ra._pObject;
			_method  = ra._method;
			return *this;
		}

		void		run()
		{
			(_pObject->*_method)();
		}

	private:
					xrunnable_adapter();

		C*			_pObject;
		callback	_method;
	};


} // namespace xcore


#endif // __XMTHREAD_RUNNABLEADAPTER_H__
