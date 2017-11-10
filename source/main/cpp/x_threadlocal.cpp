#include "xbase\x_target.h"
#include "xbase\x_debug.h"

#include "xmthread\x_threadlocal.h"
#include "xmthread\x_thread.h"


namespace xcore 
{
	xthread_ls::xthread_ls()
	{
		for (s32 i=0; i<MAX_INDEX; ++i)
			_storage[i]=0;
	}

	xthread_ls::~xthread_ls()
	{
	}

	// xthread_ls for the main thread
	static xthread_ls sMainThreadLs;

	xthread_ls* xthread_ls::sCurrent()
	{
		xthread* pThread = xthread::current();
		if (pThread)
		{
			return pThread->tls();
		}
		else
		{
			return &sMainThreadLs;
		}
	}

} // namespace xcore
