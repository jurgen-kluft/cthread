#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_namedevent.h"
#include "xthread/x_thread.h"
#include "xthread/x_runnable.h"
#include "xtime\x_datetime.h"

#include "xunittest\xunittest.h"

using xcore::xthread;
using xcore::xrunnable;
using xcore::xdatetime;


static xcore::xnevent testEvent("TestEvent");

namespace
{
	class TestEvent: public xrunnable
	{
	public:
		void run()
		{
			testEvent.wait();
			_timestamp = xdatetime::sNow();
		}

		const xdatetime& timestamp() const
		{
			return _timestamp;
		}

	private:
		xcore::xdatetime _timestamp;
	};
}


UNITTEST_SUITE_BEGIN(xnevent)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testNamedEvent)
		{
			xthread thr1;
			TestEvent te;
			thr1.start(te);
			xdatetime now = xdatetime::sNow();
			xthread::sleep(2000);
			testEvent.set();
			thr1.join();
			CHECK_TRUE (te.timestamp() > now);

			xthread thr2;
			thr2.start(te);
			now = xdatetime::sNow();
			xthread::sleep(2000);
			testEvent.set();
			thr2.join();
			CHECK_TRUE(te.timestamp() > now);
		}

	}
}
UNITTEST_SUITE_END
