#ifndef __XMTHREAD_AUTOPTR_H__
#define __XMTHREAD_AUTOPTR_H__
#include "xbase\x_target.h"
#include "xbase\x_debug.h"

namespace xcore
{
	/// xauto_ptr is a "smart" pointer for classes implementing
	/// reference counting based garbage collection.
	/// To be usable with the xauto_ptr template, a class must
	/// implement the following behaviour:
	///    A class must maintain a reference count.
	///    The constructors of the object initialize the reference
	///     count to one.
	///    The class must implement a public duplicate() method:
	///        void duplicate();
	///     that increments the reference count by one.
	///    The class must implement a public release() method:
	///        void release()
	///     that decrements the reference count by one, and,
	///     if the reference count reaches zero, deletes the
	///     object.
	///
	/// xauto_ptr works in the following way:
	///    If an xauto_ptr is assigned an ordinary pointer to
	///    an object (via the constructor or the assignment operator),
	///    it takes ownership of the object and the object's reference 
	///    count remains unchanged.
	///    If the xauto_ptr is assigned another xauto_ptr, the
	///    object's reference count is incremented by one by
	///    calling duplicate() on its object.
	///    The destructor of xauto_ptr calls release() on its
	///    object.
	/// xauto_ptr supports dereferencing with both the ->
	/// and the * operator. An attempt to dereference a null
	/// xauto_ptr results in a NullPointerException being thrown.
	/// xauto_ptr also implements all relational operators.
	/// Note that xauto_ptr allows casting of its encapsulated data types.
	template <class C>
	class xauto_ptr
	{
	public:
		xauto_ptr() : _ptr(0)
		{
		}

		xauto_ptr(C* ptr) : _ptr(ptr)
		{
		}

		xauto_ptr(C* ptr, bool shared) : _ptr(ptr)
		{
			if (shared && _ptr)
				_ptr->duplicate();
		}

		xauto_ptr(const xauto_ptr& ptr) : _ptr(ptr._ptr)
		{
			if (_ptr)
				_ptr->duplicate();
		}

		template <class Other> 
		xauto_ptr(const xauto_ptr<Other>& ptr): _ptr(const_cast<Other*>(ptr.get()))
		{
			if (_ptr) 
				_ptr->duplicate();
		}

		~xauto_ptr()
		{
			if (_ptr) 
				_ptr->release();
		}

		xauto_ptr& assign(C* ptr)
		{
			if (_ptr != ptr)
			{
				if (_ptr) _ptr->release();
				_ptr = ptr;
			}
			return *this;
		}

		xauto_ptr& assign(C* ptr, bool shared)
		{
			if (_ptr != ptr)
			{
				if (_ptr) 
					_ptr->release();
				_ptr = ptr;
				if (shared && _ptr) 
					_ptr->duplicate();
			}
			return *this;
		}

		xauto_ptr& assign(const xauto_ptr& ptr)
		{
			if (&ptr != this)
			{
				if (_ptr)
					_ptr->release();
				_ptr = ptr._ptr;
				if (_ptr) 
					_ptr->duplicate();
			}
			return *this;
		}

		template <class Other> 
		xauto_ptr& assign(const xauto_ptr<Other>& ptr)
		{
			if (ptr.get() != _ptr)
			{
				if (_ptr) 
					_ptr->release();
				_ptr = const_cast<Other*>(ptr.get());
				if (_ptr) 
					_ptr->duplicate();
			}
			return *this;
		}

		xauto_ptr& operator = (C* ptr)
		{
			return assign(ptr);
		}

		xauto_ptr& operator = (const xauto_ptr& ptr)
		{
			return assign(ptr);
		}

		template <class Other> 
		xauto_ptr& operator = (const xauto_ptr<Other>& ptr)
		{
			return assign<Other>(ptr);
		}

		void swap(xauto_ptr& ptr)
		{
			C* tmp = _ptr;
			_ptr = ptr._ptr;
			ptr._ptr = tmp;
		}

		template <class Other> 
		xauto_ptr<Other> cast() const
			/// Casts the xauto_ptr via a dynamic cast to the given type.
			/// Returns an xauto_ptr containing NULL if the cast fails.
			/// Example: (assume class Sub: public Super)
			///    xauto_ptr<Super> super(new Sub());
			///    xauto_ptr<Sub> sub = super.cast<Sub>();
			///    ASSERT (sub.get());
		{
			Other* pOther = dynamic_cast<Other*>(_ptr);
			return xauto_ptr<Other>(pOther, true);
		}

		template <class Other> 
		xauto_ptr<Other> unsafeCast() const
			/// Casts the xauto_ptr via a static cast to the given type.
			/// Example: (assume class Sub: public Super)
			///    xauto_ptr<Super> super(new Sub());
			///    xauto_ptr<Sub> sub = super.unsafeCast<Sub>();
			///    ASSERT (sub.get());
		{
			Other* pOther = static_cast<Other*>(_ptr);
			return xauto_ptr<Other>(pOther, true);
		}

		C* operator -> ()
		{
			if (_ptr)
				return _ptr;
			else
				return NULL;
		}

		const C* operator -> () const
		{
			if (_ptr)
				return _ptr;
			else
				return NULL;
		}

		C& operator * ()
		{
			ASSERT(_ptr != NULL);
			return *_ptr;
		}

		const C& operator * () const
		{
			ASSERT(_ptr != NULL);
			return *_ptr;
		}

		C* get()
		{
			return _ptr;
		}

		const C* get() const
		{
			return _ptr;
		}

		operator C* ()
		{
			return _ptr;
		}

		operator const C* () const
		{
			return _ptr;
		}

		bool operator ! () const
		{
			return _ptr == 0;
		}

		bool isNull() const
		{
			return _ptr == 0;
		}

		C* duplicate()
		{
			if (_ptr) _ptr->duplicate();
			return _ptr;
		}

		bool operator == (const xauto_ptr& ptr) const
		{
			return _ptr == ptr._ptr;
		}

		bool operator == (const C* ptr) const
		{
			return _ptr == ptr;
		}

		bool operator == (C* ptr) const
		{
			return _ptr == ptr;
		}

		bool operator != (const xauto_ptr& ptr) const
		{
			return _ptr != ptr._ptr;
		}

		bool operator != (const C* ptr) const
		{
			return _ptr != ptr;
		}

		bool operator != (C* ptr) const
		{
			return _ptr != ptr;
		}

		bool operator < (const xauto_ptr& ptr) const
		{
			return _ptr < ptr._ptr;
		}

		bool operator < (const C* ptr) const
		{
			return _ptr < ptr;
		}

		bool operator < (C* ptr) const
		{
			return _ptr < ptr;
		}

		bool operator <= (const xauto_ptr& ptr) const
		{
			return _ptr <= ptr._ptr;
		}

		bool operator <= (const C* ptr) const
		{
			return _ptr <= ptr;
		}

		bool operator <= (C* ptr) const
		{
			return _ptr <= ptr;
		}

		bool operator > (const xauto_ptr& ptr) const
		{
			return _ptr > ptr._ptr;
		}

		bool operator > (const C* ptr) const
		{
			return _ptr > ptr;
		}

		bool operator > (C* ptr) const
		{
			return _ptr > ptr;
		}

		bool operator >= (const xauto_ptr& ptr) const
		{
			return _ptr >= ptr._ptr;
		}

		bool operator >= (const C* ptr) const
		{
			return _ptr >= ptr;
		}

		bool operator >= (C* ptr) const
		{
			return _ptr >= ptr;
		}

	private:
		C* _ptr;
	};


	template <class C>
	inline void swap(xauto_ptr<C>& p1, xauto_ptr<C>& p2)
	{
		p1.swap(p2);
	}


} // namespace xcore


#endif // __XMTHREAD_AUTOPTR_H__
