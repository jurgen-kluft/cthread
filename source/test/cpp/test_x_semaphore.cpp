#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_thread.h"
#include "xthread/x_threading.h"
#include "xthread/x_semaphore.h"

#include "xunittest/xunittest.h"

using xcore::xthread;
using xcore::xsemaphore;

class SemaRunnable: public xcore::xthread_functor
{
public:
	SemaRunnable(int n, int max): _ran(false), _sema(n, max)
	{
	}
	
	void run()
	{
		_sema.wait();
		_ran = true;
	}
	
	bool ran() const
	{
		return _ran;
	}
	
	void set()
	{
		_sema.signal();
	}
	
	bool wait()
	{
		_sema.wait();
		return true;
	}

	bool try_wait(xcore::u32 ms)
	{
		return _sema.try_wait(ms);
	}

private:
	bool _ran;
	xsemaphore _sema;
};


UNITTEST_SUITE_BEGIN(xsemaphore)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testInitZero)
		{
			SemaRunnable r(0, 3);
			CHECK_TRUE (!r.wait());
			r.set();
			r.wait();
			{
				r.try_wait(100);
				// will fail
			}

			r.set();
			r.set();
			CHECK_TRUE (r.try_wait(0));
			r.wait();
			CHECK_TRUE (!r.try_wait(10));
	
			xthread t;
			t.start(&r);
			xthread::sleep(100);
			CHECK_TRUE (!r.ran());
			r.set();
			t.join();
			CHECK_TRUE (r.ran());
			CHECK_TRUE (!r.try_wait(10));
		}


		UNITTEST_TEST(testInitNonZero)
		{
			SemaRunnable r(2, 2);
			r.wait();
			CHECK_TRUE (r.try_wait(10));
			CHECK_TRUE (!r.try_wait(10));
			r.set();
			CHECK_TRUE (r.try_wait(10));
			CHECK_TRUE (!r.try_wait(10));
		}

	}
}
UNITTEST_SUITE_END
