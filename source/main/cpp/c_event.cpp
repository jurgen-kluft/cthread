#include "cthread/c_threading.h"
#include "cthread/c_event.h"

namespace ncore 
{
	void event_t::release()
	{
		threading_t::instance()->destroy_event(this);
	}

} // namespace ncore
