#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_event.h"
#include "xthread/x_thread.h"
#include "xthread/x_threading.h"
#include "xtime/x_datetime.h"

#include "xunittest\xunittest.h"

using namespace ncore;
using ncore::datetime_t;

static ncore::xevent testEvent;

namespace
{
	class TestEvent: public ncore::xthread_functor
	{
	public:
		void run()
		{
			testEvent.wait();
			_timestamp = datetime_t::sNow();
		}

		const datetime_t& timestamp() const
		{
			return _timestamp;
		}

	private:
		ncore::datetime_t _timestamp;
	};
}


extern ncore::alloc_t* gTestAllocator;


UNITTEST_SUITE_BEGIN(xevent)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testNamedEvent)
		{
			xthreading* threading = xthreading::create(gTestAllocator);

			TestEvent te;
			xthread* thr1 = threading->create_thread("test1", threading, &te, xthread::default_stacksize(), xthread::default_priority());
			thr1->start();
			datetime_t now = datetime_t::sNow();
			xthread::sleep(2000);
			testEvent.set();
			thr1->join();
			CHECK_TRUE (te.timestamp() > now);

			xthread* thr2= threading->create_thread("test2", threading, &te, xthread::default_stacksize(), xthread::default_priority());
			thr2->start();
			now = datetime_t::sNow();
			xthread::sleep(2000);
			testEvent.set();
			thr2->join();
			CHECK_TRUE(te.timestamp() > now);

			xthreading::destroy(threading);
		}

	}
}
UNITTEST_SUITE_END
