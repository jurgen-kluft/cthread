#ifndef __XMTHREAD_THREAD_WIN32_H__
#define __XMTHREAD_THREAD_WIN32_H__
#include "xbase\x_target.h"

#include "xmthread\x_Runnable.h"

#include <Windows.h>

namespace xcore
{
	#define XMTHREAD_WIN32_DEBUGGER_THREAD_NAMES

	enum
	{
		XMTHREAD_STACK_SIZE = 8192
	};

	class xthread_impl
	{
	public:	
		typedef DWORD TIDImpl;
		typedef void (*Callable)(void*);

		typedef u32 (__stdcall *Entry)(void*);

		struct CallbackData
		{
			CallbackData(): callback(0), pData(0)
			{
			}

			Callable  callback;
			void*     pData; 
		};

		enum Priority
		{
			PRIO_LOWEST_IMPL  = THREAD_PRIORITY_LOWEST,
			PRIO_LOW_IMPL     = THREAD_PRIORITY_BELOW_NORMAL,
			PRIO_NORMAL_IMPL  = THREAD_PRIORITY_NORMAL,
			PRIO_HIGH_IMPL    = THREAD_PRIORITY_ABOVE_NORMAL,
			PRIO_HIGHEST_IMPL = THREAD_PRIORITY_HIGHEST
		};

						xthread_impl();				
						~xthread_impl();

		TIDImpl			tidImpl() const;
		void			setPriorityImpl(s32 prio);
		s32				getPriorityImpl() const;
		void			setOSPriorityImpl(s32 prio);
		s32				getOSPriorityImpl() const;
		static s32		getMinOSPriorityImpl();
		static s32		getMaxOSPriorityImpl();
		void			setStackSizeImpl(s32 size);
		s32				getStackSizeImpl() const;
		void			startImpl(xrunnable& target);
		void			startImpl(Callable target, void* pData = 0);

		void			joinImpl();
		bool			joinImpl(u32 milliseconds);
		bool			isRunningImpl() const;
		static void		sleepImpl(u32 milliseconds);
		static void		yieldImpl();

		static xthread_impl*	currentImpl();
		static TIDImpl		currentTidImpl();

	protected:
		static unsigned __stdcall runnableEntry(void* pThread);
		static unsigned __stdcall callableEntry(void* pThread);

		void			createImpl(Entry ent, void* pData);
		void			threadCleanup();

	private:
		class CurrentThreadHolder
		{
		public:
			CurrentThreadHolder() : _slot(TlsAlloc())
			{
				if (_slot == TLS_OUT_OF_INDEXES)
				{
					// cannot allocate thread context key
				}
			}
			~CurrentThreadHolder()
			{
				TlsFree(_slot);
			}
			xthread_impl* get() const
			{
				return reinterpret_cast<xthread_impl*>(TlsGetValue(_slot));
			}
			void set(xthread_impl* pThread)
			{
				TlsSetValue(_slot, pThread);
			}

		private:
			DWORD _slot;
		};

		xrunnable*    _pRunnableTarget;
		CallbackData _callbackTarget;
		HANDLE       _thread;
		DWORD        _threadId;
		s32          _prio;
		s32          _stackSize;

		static CurrentThreadHolder _currentThreadHolder;
	};


	//
	// inlines
	//
	inline s32 xthread_impl::getPriorityImpl() const
	{
		return _prio;
	}


	inline s32 xthread_impl::getOSPriorityImpl() const
	{
		return _prio;
	}


	inline s32 xthread_impl::getMinOSPriorityImpl()
	{
		return PRIO_LOWEST_IMPL;
	}


	inline s32 xthread_impl::getMaxOSPriorityImpl()
	{
		return PRIO_HIGHEST_IMPL;
	}


	inline void xthread_impl::sleepImpl(u32 milliseconds)
	{
		Sleep(DWORD(milliseconds));
	}


	inline void xthread_impl::yieldImpl()
	{
		Sleep(0);
	}


	inline void xthread_impl::setStackSizeImpl(s32 size)
	{
		_stackSize = size;
	}


	inline s32 xthread_impl::getStackSizeImpl() const
	{
		return _stackSize;
	}


	inline xthread_impl::TIDImpl xthread_impl::tidImpl() const
	{
		return _threadId;
	}


} // namespace xcore


#endif // __XMTHREAD_THREAD_WIN32_H__
