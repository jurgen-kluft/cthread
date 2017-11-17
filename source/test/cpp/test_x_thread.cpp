#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"
#include "xbase/x_string_std.h"

#include "xthread/x_thread.h"
#include "xthread/x_runnable.h"
#include "xthread/x_threadtarget.h"
#include "xthread/x_event.h"

#include "xtime\x_datetime.h"
#include "xtime\x_timespan.h"

#include "xunittest\xunittest.h"

using xcore::xthread;
using xcore::xrunnable;
using xcore::xthread_target;
using xcore::xevent;
using xcore::xdatetime;
using xcore::xtimespan;
using xcore::s32;


class MyRunnable: public xrunnable
{
public:
	MyRunnable(): _ran(false)
	{
	}
	
	void run()
	{
		xthread* pThread = xthread::current();
		if (pThread)
			_threadName = pThread->name();
		_ran = true;
		_event.wait();
	}
	
	bool ran() const
	{
		return _ran;
	}
	
	const char* threadName() const
	{
		return _threadName;
	}
	
	void notify()
	{
		_event.set();
	}
	
	static void staticFunc()
	{
		++_staticVar;
	}

	static s32 _staticVar;

private:
	bool		_ran;
	const char*	_threadName;
	xevent		_event;
};


s32 MyRunnable::_staticVar = 0;


void freeFunc()
{
	++MyRunnable::_staticVar;
}


void freeFunc(void* pData)
{
	MyRunnable::_staticVar += *reinterpret_cast<s32*>(pData);
}

UNITTEST_SUITE_BEGIN(xthread)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}


		UNITTEST_TEST(testThread)
		{
			xthread thread;
			MyRunnable r;
			CHECK_TRUE (!thread.isRunning());
			thread.start(r);
			xthread::sleep(200);
			CHECK_TRUE (thread.isRunning());
			r.notify();
			thread.join();
			CHECK_TRUE (!thread.isRunning());
			CHECK_TRUE (r.ran());
			CHECK_NOT_NULL (r.threadName());
			CHECK_FALSE ( *r.threadName() == '\0' );
		}


		UNITTEST_TEST(testNamedThread)
		{
			xthread thread("MyThread");
			MyRunnable r;
			thread.start(r);
			r.notify();
			thread.join();
			CHECK_TRUE (r.ran());
			CHECK_EQUAL (r.threadName(), "MyThread");
		}


		UNITTEST_TEST(testCurrent)
		{
			CHECK_NULL(xthread::current());
		}


		UNITTEST_TEST(testThreads)
		{
			xthread thread1("Thread1");
			xthread thread2("Thread2");
			xthread thread3("Thread3");
			xthread thread4("Thread4");

			MyRunnable r1;
			MyRunnable r2;
			MyRunnable r3;
			MyRunnable r4;
			CHECK_TRUE (!thread1.isRunning());
			CHECK_TRUE (!thread2.isRunning());
			CHECK_TRUE (!thread3.isRunning());
			CHECK_TRUE (!thread4.isRunning());
			thread1.start(r1);
			xthread::sleep(200);
			CHECK_TRUE (thread1.isRunning());
			CHECK_TRUE (!thread2.isRunning());
			CHECK_TRUE (!thread3.isRunning());
			CHECK_TRUE (!thread4.isRunning());
			thread2.start(r2);
			thread3.start(r3);
			thread4.start(r4);
			xthread::sleep(200);
			CHECK_TRUE (thread1.isRunning());
			CHECK_TRUE (thread2.isRunning());
			CHECK_TRUE (thread3.isRunning());
			CHECK_TRUE (thread4.isRunning());
			r4.notify();
			thread4.join();
			CHECK_TRUE (!thread4.isRunning());
			CHECK_TRUE (thread1.isRunning());
			CHECK_TRUE (thread2.isRunning());
			CHECK_TRUE (thread3.isRunning());
			r3.notify();
			thread3.join();
			CHECK_TRUE (!thread3.isRunning());
			r2.notify();
			thread2.join();
			CHECK_TRUE (!thread2.isRunning());
			r1.notify();
			thread1.join();
			CHECK_TRUE (!thread1.isRunning());

			CHECK_TRUE (r1.ran());
			CHECK_EQUAL(r1.threadName(), "Thread1");

			CHECK_TRUE (r2.ran());
			CHECK_EQUAL(r2.threadName(), "Thread2");

			CHECK_TRUE (r3.ran());
			CHECK_EQUAL(r3.threadName(), "Thread3");

			CHECK_TRUE (r4.ran());
			CHECK_EQUAL(r4.threadName(), "Thread4");
		}


		UNITTEST_TEST(testJoin)
		{
			xthread thread;
			MyRunnable r;
			CHECK_TRUE (!thread.isRunning());
			thread.start(r);
			xthread::sleep(200);
			CHECK_TRUE (thread.isRunning());
			CHECK_TRUE (!thread.tryJoin(100));
			r.notify();
			CHECK_TRUE (thread.tryJoin(500));
			CHECK_TRUE (!thread.isRunning());
		}


		UNITTEST_TEST(testThreadTarget)
		{
			xthread_target te(&MyRunnable::staticFunc);
			xthread thread;

			CHECK_TRUE (!thread.isRunning());

			s32 tmp = MyRunnable::_staticVar;
			thread.start(te);
			thread.join();
			CHECK_TRUE (tmp + 1 == MyRunnable::_staticVar);

			xthread_target te1(freeFunc);
			CHECK_TRUE (!thread.isRunning());

			tmp = MyRunnable::_staticVar;
			thread.start(te1);
			thread.join();
			CHECK_TRUE (tmp + 1 == MyRunnable::_staticVar);
		}


		UNITTEST_TEST(testThreadFunction)
		{
			xthread thread;

			CHECK_TRUE (!thread.isRunning());

			s32 tmp = MyRunnable::_staticVar;
			thread.start(freeFunc, &tmp);
			thread.join();
			CHECK_TRUE (tmp * 2 == MyRunnable::_staticVar);

			CHECK_TRUE (!thread.isRunning());

			tmp = MyRunnable::_staticVar = 0;
			thread.start(freeFunc, &tmp);
			thread.join();
			CHECK_TRUE (0 == MyRunnable::_staticVar);
		}


		UNITTEST_TEST(testThreadStackSize)
		{
			s32 stackSize = 50000000;

			xthread thread;
			CHECK_TRUE (0x2000 == thread.getStackSize());
			thread.setStackSize(stackSize);
			CHECK_TRUE (stackSize <= thread.getStackSize());
			s32 tmp = MyRunnable::_staticVar;
			thread.start(freeFunc, &tmp);
			thread.join();
			CHECK_TRUE (tmp * 2 == MyRunnable::_staticVar);

			stackSize = 1;
			thread.setStackSize(stackSize);
		#ifdef PTHREAD_STACK_MIN
			CHECK_TRUE (PTHREAD_STACK_MIN == thread.getStackSize());
		#else
			CHECK_TRUE (stackSize >= thread.getStackSize());
		#endif
			tmp = MyRunnable::_staticVar;
			thread.start(freeFunc, &tmp);
			thread.join();
			CHECK_TRUE (tmp * 2 == MyRunnable::_staticVar);

			thread.setStackSize(0);
			CHECK_TRUE (0 == thread.getStackSize());
			tmp = MyRunnable::_staticVar;
			thread.start(freeFunc, &tmp);
			thread.join();
			CHECK_TRUE (tmp * 2 == MyRunnable::_staticVar);
		}


		UNITTEST_TEST(testSleep)
		{
			xdatetime start = xdatetime::sNow();
			xthread::sleep(200);
			xdatetime end = xdatetime::sNow();
			xtimespan elapsed = end - start;
			CHECK_TRUE (elapsed.totalMilliseconds() >= 190 && elapsed.totalMilliseconds() < 250);
		}

	}
}
UNITTEST_SUITE_END
