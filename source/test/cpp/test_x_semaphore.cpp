#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_allocator.h"

#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "cthread/c_semaphore.h"

#include "cunittest/xunittest.h"

using ncore::xthread;
using ncore::xsemaphore;

class SemaRunnable: public ncore::xthread_functor
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

	bool try_wait(ncore::u32 ms)
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
