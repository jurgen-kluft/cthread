#include "xbase\x_target.h"
#include "xbase\x_string_std.h"

#include "xmthread\x_thread.h"
#include "xmthread\x_mutex.h"
#include "xmthread\x_threadlocal.h"

namespace xcore 
{
	static void copy_str(char* to, s32 max_len, const char* from)
	{
		for (s32 i=0; i<max_len; ++i)
		{
			to[i] = from[i];
			if (from[i] == '\0')
				break;
		}
	}

	xthread::xthread()
		: _id(uniqueId())
	{
		_name[0] = '\0';
		_name[1] = '\0';
		makeName(_name, sizeof(_name));
	}

	xthread::xthread(const char* name)
		: _id(uniqueId())
	{
		copy_str(_name, sizeof(_name), name);
	}

	xthread::~xthread()
	{
	}

	void xthread::setPriority(Priority prio)
	{
		setPriorityImpl(prio);
	}

	xthread::Priority xthread::getPriority() const
	{
		return Priority(getPriorityImpl());
	}

	void xthread::start(xrunnable& target)
	{
		startImpl(target);
	}

	void xthread::start(Callable target, void* pData)
	{
		startImpl(target, pData);
	}

	void xthread::join()
	{
		joinImpl();
	}

	void xthread::join(u32 milliseconds)
	{
		if (!joinImpl(milliseconds))
		{
			// timeout
		}
	}


	bool xthread::tryJoin(u32 milliseconds)
	{
		return joinImpl(milliseconds);
	}

	xthread_ls* xthread::tls()
	{
		return &_pTLS;
	}

	void xthread::makeName(char* str, s32 max_len)
	{
		x_sprintf(str, max_len-1, "#%d", _id);
	}

	namespace
	{
		static xfastmutex uniqueIdMutex;
	}


	s32 xthread::uniqueId()
	{
		xfastmutex::xscoped_lock lock(uniqueIdMutex);

		static unsigned count = 0;
		++count;
		return count;
	}


	void xthread::setName(const char* name)
	{
		xfastmutex::xscoped_lock lock(_mutex);
		copy_str(_name, sizeof(_name), name);
	}


} // namespace xcore
