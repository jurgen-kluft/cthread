#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"

#include "xmthread\x_threadpool.h"
#include "xmthread\x_runnableadapter.h"
#include "xmthread\x_thread.h"
#include "xmthread\x_event.h"

#include "xunittest\xunittest.h"

using xcore::xthreadpool;
using xcore::xrunnable_adapter;
using xcore::xthread;

class ThreadPoolTest
{
public:
	ThreadPoolTest() 
		: _event(false)		// Set xevent to non auto-reset
		, _count(0)
	{
	}

	void				init();
	void				count();

	xcore::xfastmutex	_mutex;
	xcore::xevent		_event;
	xcore::s32			_count;
};

void ThreadPoolTest::init()
{
	_event.reset();
	_count = 0;
}

void ThreadPoolTest::count()
{
	_event.wait();
	for (int i = 0; i < 10000; ++i)
	{
		_mutex.lock();
		++_count;
		_mutex.unlock();
	}
}

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xthreadpool)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testThreadPool)
		{
			xthreadpool pool(gTestAllocator, 2, 4, 3);
			pool.setStackSize(1);

			ThreadPoolTest tptest;
			tptest.init();

			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 0);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 4);

			xrunnable_adapter<ThreadPoolTest> ra(tptest, &ThreadPoolTest::count);
			CHECK_TRUE (pool.start(ra))
			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 1);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 3);

			CHECK_TRUE (pool.start(ra))
			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 2);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 2);

			CHECK_TRUE (pool.start(ra))
			CHECK_TRUE (pool.allocated() == 3);
			CHECK_TRUE (pool.used() == 3);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 1);

			CHECK_TRUE (pool.start(ra))
			CHECK_TRUE (pool.allocated() == 4);
			CHECK_TRUE (pool.used() == 4);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 0);

			{
				CHECK_FALSE(pool.start(ra));
				// thread pool exhausted
			}	
	
			tptest._event.set(); // all threads go!!!
			pool.joinAll();
	
			CHECK_TRUE (tptest._count == 40000);
	
			CHECK_TRUE (pool.allocated() == 4);
			CHECK_TRUE (pool.used() == 0);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 4);
	
			xthread::sleep(4000);

			pool.collect();
			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 0);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 4);
	
			tptest._count = 0;
			tptest._event.reset();

			CHECK_TRUE (pool.start(ra))

			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 1);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 3);

			CHECK_TRUE (pool.start(ra))

			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 2);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 2);
			tptest._event.set(); // go!!!
			pool.joinAll();

			CHECK_TRUE (tptest._count == 20000);
	
			CHECK_TRUE (pool.allocated() == 2);
			CHECK_TRUE (pool.used() == 0);
			CHECK_TRUE (pool.capacity() == 4);
			CHECK_TRUE (pool.available() == 4);
		}

	}
}
UNITTEST_SUITE_END
