#ifndef __XMTHREAD_REFCOUNTEDOBJECT_H__
#define __XMTHREAD_REFCOUNTEDOBJECT_H__
#include "xbase\x_target.h"

#include "xmthread\x_atomiccounter.h"

namespace xcore 
{
	// A base class for objects that employ
	// reference counting based garbage collection.
	//
	// Reference-counted objects inhibit construction
	// by copying and assignment.
	class xrefobject
	{
	public:
					xrefobject();
		/// Creates the xrefobject.
		/// The initial reference count is one.

		void		duplicate() const;
		/// Increments the object's reference count.

		void		release() const;
		/// Decrements the object's reference count
		/// and deletes the object if the count
		/// reaches zero.

		s32			referenceCount() const;
		/// Returns the reference count.

	protected:
		virtual		~xrefobject();
		/// Destroys the xrefobject.

	private:
					xrefobject(const xrefobject&);
					xrefobject& operator = (const xrefobject&);

		mutable xatomcnt _counter;
	};


	//
	// inlines
	//
	inline s32 xrefobject::referenceCount() const
	{
		return _counter.value();
	}


	inline void xrefobject::duplicate() const
	{
		++_counter;
	}


	inline void xrefobject::release() const
	{
		if (--_counter == 0) 
			delete this;
	}


} // namespace xcore


#endif // __XMTHREAD_REFCOUNTEDOBJECT_H__
