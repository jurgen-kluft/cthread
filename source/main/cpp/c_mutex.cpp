#include "cthread/c_threading.h"
#include "cthread/c_mutex.h"

namespace ncore 
{
	void mutex_t::release()
	{
		threading_t::instance()->destroy_mutex(this);
	}

} // namespace ncore
