#ifndef __XMTHREAD_SHAREDPTR_H__
#define __XMTHREAD_SHAREDPTR_H__
#include "xbase\x_target.h"
#include "xbase\x_debug.h"

#include "xmthread\x_atomiccounter.h"

namespace xcore
{
	/// Simple xrefcounter object, does not delete itself when count reaches 0.
	class xrefcounter
	{
	public:
					xrefcounter() : _cnt(1) { }

		void		duplicate()					{ ++_cnt; }
		s32			release()					{ return --_cnt; }
		s32			referenceCount() const		{ return _cnt.value(); }

	private:
		xatomcnt _cnt;
	};

	/// The default release policy for xshared_ptr, which
	/// simply uses the delete operator to delete an object.
	template <class C>
	class xrelease_policy
	{
	public:
		/// Delete the object.
		/// Note that pObj can be 0.
		static void release(C* pObj)
		{
			delete pObj;
		}
	};

	/// The call release policy for xshared_ptr, which
	/// simply calls the release() function on the object.
	template <class C>
	class xcallrelease_policy
	{
	public:
		/// Note that pObj can be 0.
		static void release(C* pObj)
		{
			if (pObj!=NULL) pObj->release();
		}
	};

	/// The release policy for xshared_ptr holding arrays.
	template <class C>
	class xreleasearray_policy
	{
	public:
		/// Delete the object.
		/// Note that pObj can be 0.
		static void release(C* pObj)
		{
			delete [] pObj;
		}
	};


	/// xshared_ptr is a "smart" pointer for classes implementing
	/// reference counting based garbage collection.
	/// xshared_ptr is thus similar to xauto_ptr. Unlike the
	/// xauto_ptr template, which can only be used with
	/// classes that support reference counting, xshared_ptr
	/// can be used with any class. For this to work, a
	/// xshared_ptr manages a reference count for the object
	/// it manages.
	/// 
	/// xshared_ptr works in the following way:
	/// If an xshared_ptr is assigned an ordinary pointer to
	/// an object (via the constructor or the assignment operator),
	/// it takes ownership of the object and the object's reference 
	/// count is initialized to one.
	/// If the xshared_ptr is assigned another xshared_ptr, the
	/// object's reference count is incremented by one.
	/// The destructor of xshared_ptr decrements the object's
	/// reference count by one and deletes the object if the
	/// reference count reaches zero.
	/// xshared_ptr supports dereferencing with both the ->
	/// and the * operator. An attempt to dereference a null
	/// xshared_ptr results in a NullPointerException being thrown.
	/// xshared_ptr also implements all relational operators and
	/// a cast operator in case dynamic casting of the encapsulated data types
	/// is required.
	template <class C, class RC = xrefcounter, class RP = xrelease_policy<C> >
	class xshared_ptr
	{
	public:
		xshared_ptr() : _pCounter(new RC), _ptr(0)
		{
		}

		xshared_ptr(C* ptr): _pCounter(new RC), _ptr(ptr)
		{
		}

		template <class Other, class OtherRP> 
		xshared_ptr(const xshared_ptr<Other, RC, OtherRP>& ptr): _pCounter(ptr._pCounter), _ptr(const_cast<Other*>(ptr.get()))
		{
			_pCounter->duplicate();
		}

		xshared_ptr(const xshared_ptr& ptr): _pCounter(ptr._pCounter), _ptr(ptr._ptr)
		{
			_pCounter->duplicate();
		}

		~xshared_ptr()
		{
			release();
		}

		xshared_ptr& assign(C* ptr)
		{
			if (get() != ptr)
			{
				RC* pTmp = new RC;
				release();
				_pCounter = pTmp;
				_ptr = ptr;
			}
			return *this;
		}

		xshared_ptr& assign(const xshared_ptr& ptr)
		{
			if (&ptr != this)
			{
				xshared_ptr tmp(ptr);
				swap(tmp);
			}
			return *this;
		}

		template <class Other, class OtherRP>
		xshared_ptr& assign(const xshared_ptr<Other, RC, OtherRP>& ptr)
		{
			if (ptr.get() != _ptr)
			{
				xshared_ptr tmp(ptr);
				swap(tmp);
			}
			return *this;
		}

		xshared_ptr& operator = (C* ptr)
		{
			return assign(ptr);
		}

		xshared_ptr& operator = (const xshared_ptr& ptr)
		{
			return assign(ptr);
		}

		template <class Other, class OtherRP>
		xshared_ptr& operator = (const xshared_ptr<Other, RC, OtherRP>& ptr)
		{
			return assign<Other>(ptr);
		}

		void swap(xshared_ptr& ptr)
		{
			C* tmp1 = _ptr;
			_ptr = ptr._ptr;
			ptr._ptr = tmp1;

			RC* tmp2 = _pCounter;
			_pCounter = ptr._pCounter;
			ptr._pCounter = tmp2;
		}

		/// Casts the xshared_ptr via a static cast to the given type.
		/// Example: (assume class Sub: public Super)
		///    xshared_ptr<Super> super(new Sub());
		///    xshared_ptr<Sub> sub = super.unsafeCast<Sub>();
		///    ASSERT(sub.get());
		template <class Other> 
		xshared_ptr<Other, RC, RP> unsafeCast() const
		{
			Other* pOther = static_cast<Other*>(_ptr);
			return xshared_ptr<Other, RC, RP>(_pCounter, pOther);
		}

		C*			operator -> ()								{ return deref(); }
		const C*	operator -> () const						{ return deref(); }
		C&			operator * ()								{ return *deref(); }
		const C&	operator * () const							{ return *deref(); }
		C*			get()										{ return _ptr; }
		const C*	get() const									{ return _ptr; }
		inline		operator C* ()								{ return _ptr; }
		inline		operator const C* () const					{ return _ptr; }
		bool		operator ! () const							{ return _ptr == 0; }
		bool		isNull() const								{ return _ptr == 0; }
		bool		operator == (const xshared_ptr& ptr) const	{ return get() == ptr.get(); }
		bool		operator == (const C* ptr) const			{ return get() == ptr; }
		bool		operator == (C* ptr) const					{ return get() == ptr; }
		bool		operator != (const xshared_ptr& ptr) const	{ return get() != ptr.get(); }
		bool		operator != (const C* ptr) const			{ return get() != ptr; }
		bool		operator != (C* ptr) const					{ return get() != ptr; }
		bool		operator < (const xshared_ptr& ptr) const		{ return get() < ptr.get(); }
		bool		operator < (const C* ptr) const				{ return get() < ptr; }
		bool		operator < (C* ptr) const					{ return get() < ptr; }
		bool		operator <= (const xshared_ptr& ptr) const	{ return get() <= ptr.get(); }
		bool		operator <= (const C* ptr) const			{ return get() <= ptr; }
		bool		operator <= (C* ptr) const					{ return get() <= ptr; }
		bool		operator > (const xshared_ptr& ptr) const		{ return get() > ptr.get(); }
		bool		operator > (const C* ptr) const				{ return get() > ptr; }
		bool		operator > (C* ptr) const					{ return get() > ptr; }
		bool		operator >= (const xshared_ptr& ptr) const	{ return get() >= ptr.get(); }
		bool		operator >= (const C* ptr) const			{ return get() >= ptr; }
		bool		operator >= (C* ptr) const					{ return get() >= ptr; }
		s32			referenceCount() const						{ return _pCounter->referenceCount(); }

	private:
		C*			deref() const
		{
			ASSERT(_ptr!=NULL);
			return _ptr;
		}

		void		release()
		{
			ASSERT(_pCounter);
			s32 i = _pCounter->release();
			if (i == 0)
			{
				RP::release(_ptr);
				_ptr = 0;

				delete _pCounter;
				_pCounter = 0;
			}
		}

		/// for cast operation
		xshared_ptr(RC* pCounter, C* ptr): _pCounter(pCounter), _ptr(ptr)
		{
			ASSERT (_pCounter);
			_pCounter->duplicate();
		}

	private:
		RC*		_pCounter;
		C*		_ptr;

		template <class OtherC, class OtherRC, class OtherRP> friend class xshared_ptr;
	};


	template <class C, class RC, class RP>
	inline void swap(xshared_ptr<C, RC, RP>& p1, xshared_ptr<C, RC, RP>& p2)
	{
		p1.swap(p2);
	}


} // namespace xcore


#endif // __XMTHREAD_SHAREDPTR_H__
