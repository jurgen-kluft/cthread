#ifndef __XMTHREAD_THREADING_H__
#define __XMTHREAD_THREADING_H__
#include "xbase/x_target.h"

#include "xthread/x_thread.h"

namespace xcore
{
    class xalloc;

	class xthread_functor
	{
	public:	
		virtual			~xthread_functor() {}
		virtual void	run() = 0;
	};

    class xthread;
    class xmutex;
    class xevent;
    class xsemaphore;
    class xthreading_data;

	class xthreading
	{
	public:
		static xthreading*	create(xalloc* allocator, u32 max_threads = 32, u32 max_mutex = 32, u32 max_event = 32, u32 max_semaphore = 32);

		xthread*            create_thread(const char* name, void* arg, xthread_functor* f, u32 stack_size, xthread::e_priority priority);
		xmutex*				create_mutex();
		xevent*				create_event();
		xsemaphore*			create_semaphore();

		void				destroy_thread(xthread*);
		void				destroy_mutex(xmutex*);
		void				destroy_event(xevent*);
		void				destroy_semaphore(xsemaphore*);

		void				join(xthread*);
		bool				join(xthread*, u32 milliseconds);

		static xthread*		current();
		static void			sleep(u32 milliseconds);
		static void			yield();
		static void			exit();

	private:
							xthreading();
							~xthreading();						/// Destroys the thread.
							xthreading(const xthreading&);
		xthreading&			operator = (const xthreading&);

		xthreading_data* m_data;
	};

} // namespace xcore


#endif 