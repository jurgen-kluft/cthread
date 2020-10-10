#include "xbase/x_target.h"
#include "xbase/x_string_ascii.h"

#include "xthread/x_threading.h"
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



} // namespace xcore
