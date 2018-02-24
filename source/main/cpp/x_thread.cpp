#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"

#include "xthread/x_thread.h"
#include "xthread/x_mutex.h"

#include <atomic>

namespace xcore 
{
	static void sMakeName(xuchars& str, xthread::tid_t id)
	{
		ascii::sprintf(str, xcuchars("#%d"), x_va(id));
	}

	static s32 sUniqueId()
	{
		static std::atomic<int> count(0);
		return count++;
	}

	xthread::xthread()
		: ithread(ithread::config())
	{
		m_threadTid = (sUniqueId());
		m_name[0] = '\0';
		m_name[1] = '\0';
		xuchars name(m_name, &m_name[sizeof(m_name) - 1]);
		sMakeName(name, m_threadTid);
	}

	xthread::xthread(const char* _name)
		: ithread(ithread::config())
	{
		m_threadTid = (sUniqueId());
		xuchars name(m_name, &m_name[sizeof(m_name) - 1]);
		ascii::copy(name, xcuchars(_name));
	}

	xthread::xthread(config const& cnfg)
		: ithread(cnfg)
	{
		m_threadTid = (sUniqueId());
		m_name[0] = '\0';
		m_name[1] = '\0';
		xuchars name(m_name, &m_name[sizeof(m_name) - 1]);
		sMakeName(name, m_threadTid);
	}

	xthread::~xthread()
	{
	}

	void xthread::set_priority(e_priority prio)
	{
		thread_set_priority(prio);
	}

	xthread::e_priority xthread::get_priority() const
	{
		return thread_get_priority();
	}

	void xthread::start(xthread_functor* f)
	{
		thread_start(f);
	}

	void xthread::join()
	{
		thread_join();
	}

	bool xthread::join(u32 milliseconds)
	{
		if (!thread_join(milliseconds))
		{
			// timeout
			return false;
		}
		return true;
	}

	void xthread::set_name(const char* name)
	{
		ascii::copy(xuchars(m_name), xcuchars(name));
	}


} // namespace xcore
