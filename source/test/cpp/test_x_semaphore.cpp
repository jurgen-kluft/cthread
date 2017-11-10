#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"

#include "xmthread\x_thread.h"
#include "xmthread\x_runnable.h"
#include "xmthread\x_semaphore.h"

#include "xunittest\xunittest.h"

using xcore::xthread;
using xcore::xrunnable;
using xcore::xsemaphore;

class SemaRunnable: public xrunnable
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
		_sema.set();
	}
	
	void wait()
	{
		_sema.wait();
	}
	
	void wait(long milliseconds)
	{
		_sema.wait(milliseconds);
	}

	bool tryWait(long milliseconds)
	{
		return _sema.tryWait(milliseconds);
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
			CHECK_TRUE (!r.tryWait(10));
			r.set();
			r.wait();
			{
				r.wait(100);
				// will fail
			}

			r.set();
			r.set();
			CHECK_TRUE (r.tryWait(0));
			r.wait();
			CHECK_TRUE (!r.tryWait(10));
	
			xthread t;
			t.start(r);
			xthread::sleep(100);
			CHECK_TRUE (!r.ran());
			r.set();
			t.join();
			CHECK_TRUE (r.ran());
			CHECK_TRUE (!r.tryWait(10));
		}


		UNITTEST_TEST(testInitNonZero)
		{
			SemaRunnable r(2, 2);
			r.wait();
			CHECK_TRUE (r.tryWait(10));
			CHECK_TRUE (!r.tryWait(10));
			r.set();
			CHECK_TRUE (r.tryWait(10));
			CHECK_TRUE (!r.tryWait(10));
		}

	}
}
UNITTEST_SUITE_END
