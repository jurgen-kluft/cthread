#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_thread.h"
#include "xthread/x_runnable.h"
#include "xthread/x_condition.h"
#include "xthread/x_mutex.h"

#include "xunittest\xunittest.h"

using xcore::xthread;
using xcore::xrunnable;
using xcore::xcondition;
using xcore::xmutex;

namespace
{
	class WaitRunnable: public xrunnable
	{
	public:
		WaitRunnable(xcondition& cond, xmutex& mutex)
			: _ran(false)
			, _cond(cond)
			, _mutex(mutex)
		{
		}
		
		void run()
		{
			_mutex.lock();
			_cond.wait(_mutex);
			_mutex.unlock();
			_ran = true;
		}
		
		bool ran() const
		{
			return _ran;
		}
			
	private:
		bool			_ran;
		xcondition&		_cond;
		xmutex&			_mutex;
	};
	
	class TryWaitRunnable: public xrunnable
	{
	public:
		TryWaitRunnable(xcondition& cond, xmutex& mutex)
			: _ran(false)
			, _cond(cond)
			, _mutex(mutex)
		{
		}
		
		void run()
		{
			_mutex.lock();
			if (_cond.tryWait(_mutex, 10000))
				_ran = true;
			_mutex.unlock();
		}
		
		bool ran() const
		{
			return _ran;
		}
			
	private:
		bool			_ran;
		xcondition&		_cond;
		xmutex&			_mutex;
	};

}

UNITTEST_SUITE_BEGIN(xcondition)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}


		UNITTEST_TEST(testSignal)
		{
			xcondition	cond;
			xmutex		mtx;
			WaitRunnable	r1(cond, mtx);
			WaitRunnable	r2(cond, mtx);
	
			xthread t1;
			xthread t2;
	
			t1.start(r1);
			xthread::sleep(200);
			t2.start(r2);
	
			CHECK_TRUE (!r1.ran());
			CHECK_TRUE (!r2.ran());
	
			cond.signal();
	
			t1.join();
			CHECK_TRUE (r1.ran());
	
			CHECK_TRUE (!t2.tryJoin(200));
	
			cond.signal();
	
			t2.join();

			CHECK_TRUE (r2.ran());
		}

		UNITTEST_TEST(testBroadcast)
		{
			xcondition	cond;
			xmutex		mtx;
			WaitRunnable	r1(cond, mtx);
			WaitRunnable	r2(cond, mtx);
			TryWaitRunnable	r3(cond, mtx);
	
			xthread t1;
			xthread t2;
			xthread t3;
	
			t1.start(r1);
			xthread::sleep(200);
			t2.start(r2);
			xthread::sleep(200);
			t3.start(r3);
	
			CHECK_TRUE (!r1.ran());
			CHECK_TRUE (!r2.ran());
			CHECK_TRUE (!r3.ran());
	
			cond.signal();
			t1.join();
	
			CHECK_TRUE (r1.ran());
			CHECK_TRUE (!t2.tryJoin(500));
			CHECK_TRUE (!t3.tryJoin(500));
	
			cond.broadcast();
	
			t2.join();
			t3.join();
	
			CHECK_TRUE (r2.ran());
			CHECK_TRUE (r3.ran());
		}

	}
}
UNITTEST_SUITE_END
