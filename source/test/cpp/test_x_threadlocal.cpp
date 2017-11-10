#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"

#include "xmthread\x_threadlocal.h"
#include "xmthread\x_thread.h"
#include "xmthread\x_runnable.h"

#include "xunittest\xunittest.h"


using xcore::xthread_ls;
using xcore::xthread;
using xcore::xrunnable;
using xcore::s32;

class TLTestRunnable: public xrunnable
{
public:
	TLTestRunnable(s32 n): _n(n)
	{
	}

	void run()
	{
		s32& count = xthread_ls::sGet<s32>();
		count = 0;
		for (s32 i = 0; i < _n; ++i)
			++(count);
		_result = count;
	}
	
	s32 result()
	{
		return _result;
	}
	
private:
	s32					_n;
	s32					_result;
};


struct TLTestStruct
{
	TLTestStruct() : i(0), s("") {}

	s32			i;
	const char*	s;
};

extern xcore::x_iallocator* gTestAllocator;

UNITTEST_SUITE_BEGIN(xthread_ls)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testIntSet1)
		{
			xcore::xthreadls<s32> a;
			a = 111;
		}
		
		UNITTEST_TEST(testIntSet2)
		{
			xcore::xthreadls<s32> a(222);
		}

		UNITTEST_TEST(testIntSet3)
		{
			xcore::xthreadls<s32> a;
		}

		UNITTEST_TEST(testIntGet)
		{
			xcore::xthreadls<s32> a;
			CHECK_EQUAL(222, (s32)a);
		}

		UNITTEST_TEST(testAccessors)
		{
			TLTestStruct testStruct;
			xthread_ls::sGet<TLTestStruct*>() = &testStruct;

			testStruct.i = 100;
			testStruct.s = "foo";

			TLTestStruct* ts = (TLTestStruct*)xthread_ls::sGet<TLTestStruct*>();

			CHECK_EQUAL (100, ts->i);
			CHECK_EQUAL ("foo", ts->s);

			xthread_ls::sGet<s32>() = 99;
			s32 i = xthread_ls::sCurrent()->get<s32>();
			CHECK_EQUAL(99, i);
		}

		UNITTEST_TEST(testLocality)
		{
			TLTestRunnable r1(5000);
			TLTestRunnable r2(7500);
			TLTestRunnable r3(6000);
			xthread t1;
			xthread t2;
			xthread t3;
			t1.start(r1);
			t2.start(r2);
			t3.start(r3);
			t1.join();
			t2.join();
			t3.join();
	
			CHECK_TRUE (r1.result() == 5000);
			CHECK_TRUE (r2.result() == 7500);
			CHECK_TRUE (r3.result() == 6000);
		}

	}
}
UNITTEST_SUITE_END
