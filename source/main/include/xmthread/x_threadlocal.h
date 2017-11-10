#ifndef __XMTHREAD_THREADLOCAL_H__
#define __XMTHREAD_THREADLOCAL_H__
#include "xbase\x_target.h"

namespace xcore
{
	namespace xtlsi
	{
		class xitype2idx
		{
			template <typename T>
			friend struct xtype2idx;

			static const s32 next()
			{
				static s32 id = 0; 
				return id++;
			}
		};

		template <typename T>
		struct xtype2idx
		{
			static const s32 value() 
			{
				static const s32 id = xitype2idx::next();
				return id;
			}
		};
	}


	/// This class manages the local storage for each thread.
	/// Never use this class directly, always use the
	/// ThreadLocal template for managing thread local storage.
	class xthread_ls
	{
		enum
		{
			MAX_INDEX = 8
		};

	public:
		/// Creates the TLS.
					xthread_ls();

		/// Deletes the TLS.
					~xthread_ls();

		template<typename T>
		static T&	sGet()
		{
			ASSERT(sizeof(T) <= sizeof(void*));
			s32 idx = xtlsi::xtype2idx<T>::value();
			ASSERT(idx>=0 && idx<MAX_INDEX);
			xthread_ls* ls = sCurrent();
			return *((T*)&ls->_storage[idx]);
		}

		/// Returns the maximum slot index
		inline u32	size() const						{ return MAX_INDEX; }

		/// Returns the slot value associated with the index
		template<typename T>
		T const&	get() const
		{
			ASSERT(sizeof(T) <= sizeof(void*));
			s32 idx = xtlsi::xtype2idx<T>::value();
			ASSERT(idx>=0 && idx<MAX_INDEX);
			return *((T*)&_storage[idx]); 
		}

		template<typename T>
		T&			get()
		{
			ASSERT(sizeof(T) <= sizeof(void*));
			s32 idx = xtlsi::xtype2idx<T>::value();
			ASSERT(idx>=0 && idx<MAX_INDEX);
			return *((T*)&_storage[idx]); 
		}

		/// Returns the TLS object for the current thread
		/// (which may also be the main thread).
		static xthread_ls* sCurrent();

	private:
		friend class xthread;
		void*		_storage[MAX_INDEX];
	};

	template<typename T>
	class xthreadls
	{
	public:
		inline		xthreadls()
		{
			// Do not initialize the value, the default constructor is supposed
			// to do nothing!
		}

		inline		xthreadls(T const& value)
		{
			xthread_ls* ls = xthread_ls::sCurrent();
			ls->get<T>() = value;
		}

		inline		operator T () const
		{
			xthread_ls* ls = xthread_ls::sCurrent();
			return ls->get<T>();
		}

		inline xthreadls<T>&	operator = (T const& value)
		{
			xthread_ls* ls = xthread_ls::sCurrent();
			ls->get<T>() = value;
			return *this;
		}
	};


} // namespace xcore


#endif // __XMTHREAD_THREADLOCAL_H__
