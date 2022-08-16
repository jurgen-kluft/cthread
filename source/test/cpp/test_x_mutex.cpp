#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_mutex.h"
#include "xthread/x_thread.h"
#include "xthread/x_threading.h"
#include "xtime/x_datetime.h"

#include "xunittest/xunittest.h"

using ncore::xthread;
using ncore::datetime_t;


static ncore::xmutex testMutex;


namespace
{
	class TestLock: public ncore::xthread_functor
	{
	public:
		void run()
		{
			testMutex.lock();
			_timestamp = datetime_t::sNow();
			testMutex.unlock();
		}

		const datetime_t& timestamp() const
		{
			return _timestamp;
		}

	private:
		datetime_t _timestamp;
	};

	class TestTryLock: public ncore::xthread_functor
	{
	public:
		TestTryLock(): _locked(false)
		{
		}
		
		void run()
		{
			if (testMutex.tryLock())
			{
				_locked = true;
				testMutex.unlock();
			}
		}

		bool locked() const
		{
			return _locked;
		}

	private:
		bool _locked;
	};
}

UNITTEST_SUITE_BEGIN(xmutex)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testLock)
		{
			testMutex.lock();
			xthread thr;
			TestLock tl;
			thr.start(&tl);
			datetime_t now = datetime_t::sNow();
			xthread::sleep(2000);
			testMutex.unlock();
			thr.join();
			CHECK_TRUE (tl.timestamp() > now);
		}


		UNITTEST_TEST(testTryLock)
		{
			xthread thr1;
			TestTryLock ttl1;
			thr1.start(&ttl1);
			thr1.join();
			CHECK_TRUE (ttl1.locked());
	
			testMutex.lock();
			xthread thr2;
			TestTryLock ttl2;
			thr2.start(&ttl2);
			thr2.join();
			testMutex.unlock();
			CHECK_TRUE (!ttl2.locked());
		}

	}
}
UNITTEST_SUITE_END
