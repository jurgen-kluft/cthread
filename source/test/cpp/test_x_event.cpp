#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_allocator.h"

#include "cthread/c_event.h"
#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "ctime/c_datetime.h"

#include "cunittest\cunittest.h"

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
			cthread* thr1 = threading->create_thread("test1", threading, &te, cthread::default_stacksize(), cthread::default_priority());
			thr1->start();
			datetime_t now = datetime_t::sNow();
			cthread::sleep(2000);
			testEvent.set();
			thr1->join();
			CHECK_TRUE (te.timestamp() > now);

			cthread* thr2= threading->create_thread("test2", threading, &te, cthread::default_stacksize(), cthread::default_priority());
			thr2->start();
			now = datetime_t::sNow();
			cthread::sleep(2000);
			testEvent.set();
			thr2->join();
			CHECK_TRUE(te.timestamp() > now);

			xthreading::destroy(threading);
		}

	}
}
UNITTEST_SUITE_END
