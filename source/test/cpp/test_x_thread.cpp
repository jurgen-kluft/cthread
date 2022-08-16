#include "cbase/c_target.h"
#include "cbase/c_debug.h"
#include "cbase/c_allocator.h"

#include "cthread/c_thread.h"
#include "cthread/c_threading.h"

#include "cthread/c_event.h"

#include "ctime/c_datetime.h"
#include "ctime/c_timespan.h"

#include "cunittest/xunittest.h"

using ncore::xthread;
using ncore::xevent;
using ncore::datetime_t;
using ncore::timespan_t;
using ncore::s32;


class MyRunnable: public ncore::xthread_functor
{
public:
	MyRunnable(): _ran(false)
	{
	}
	
	void run()
	{
		xthread* pThread = xthread::current();
		if (pThread)
			_threadName = pThread->get_name();
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
			CHECK_TRUE (!thread.is_running());
			thread.start(&r);
			xthread::sleep(200);
			CHECK_TRUE (thread.is_running());
			r.notify();
			thread.join();
			CHECK_TRUE (!thread.is_running());
			CHECK_TRUE (r.ran());
			CHECK_NOT_NULL (r.threadName());
			CHECK_FALSE ( *r.threadName() == '\0' );
		}


		UNITTEST_TEST(testNamedThread)
		{
			xthread thread("MyThread");
			MyRunnable r;
			thread.start(&r);
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
			CHECK_TRUE (!thread1.is_running());
			CHECK_TRUE (!thread2.is_running());
			CHECK_TRUE (!thread3.is_running());
			CHECK_TRUE (!thread4.is_running());
			thread1.start(&r1);
			xthread::sleep(200);
			CHECK_TRUE (thread1.is_running());
			CHECK_TRUE (!thread2.is_running());
			CHECK_TRUE (!thread3.is_running());
			CHECK_TRUE (!thread4.is_running());
			thread2.start(&r2);
			thread3.start(&r3);
			thread4.start(&r4);
			xthread::sleep(200);
			CHECK_TRUE (thread1.is_running());
			CHECK_TRUE (thread2.is_running());
			CHECK_TRUE (thread3.is_running());
			CHECK_TRUE (thread4.is_running());
			r4.notify();
			thread4.join();
			CHECK_TRUE (!thread4.is_running());
			CHECK_TRUE (thread1.is_running());
			CHECK_TRUE (thread2.is_running());
			CHECK_TRUE (thread3.is_running());
			r3.notify();
			thread3.join();
			CHECK_TRUE (!thread3.is_running());
			r2.notify();
			thread2.join();
			CHECK_TRUE (!thread2.is_running());
			r1.notify();
			thread1.join();
			CHECK_TRUE (!thread1.is_running());

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
			CHECK_TRUE (!thread.is_running());
			thread.start(&r);
			xthread::sleep(200);
			CHECK_TRUE (thread.is_running());
			CHECK_TRUE (!thread.join(100));
			r.notify();
			CHECK_TRUE (thread.join(500));
			CHECK_TRUE (!thread.is_running());
		}


		UNITTEST_TEST(testThreadFunction)
		{
			xthread thread;

			CHECK_TRUE (!thread.is_running());

			MyRunnable f;

			s32 tmp = MyRunnable::_staticVar;
			thread.start(&f);
			thread.join();
			CHECK_TRUE (tmp * 2 == MyRunnable::_staticVar);

			CHECK_TRUE (!thread.is_running());

			tmp = MyRunnable::_staticVar = 0;
			thread.start(&f);
			thread.join();
			CHECK_TRUE (0 == MyRunnable::_staticVar);
		}


		UNITTEST_TEST(testThreadStackSize)
		{
			s32 stackSize = 50000000;

			xthread::config cnfg;
			cnfg.m_stack_size = stackSize;
			xthread thread(cnfg);
			CHECK_TRUE (stackSize == thread.get_stacksize());
		}

		UNITTEST_TEST(testSleep)
		{
			datetime_t start = datetime_t::sNow();
			xthread::sleep(200);
			datetime_t end = datetime_t::sNow();
			timespan_t elapsed = end - start;
			CHECK_TRUE (elapsed.totalMilliseconds() >= 190 && elapsed.totalMilliseconds() < 250);
		}

	}
}
UNITTEST_SUITE_END
