#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"

#include "cthread/c_mutex.h"
#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "ctime/c_datetime.h"

#include "cunittest/cunittest.h"
#include "cthread/test_allocator.h"

using namespace ncore;

namespace
{
	class TestLock: public ncore::thread_functor
	{
	public:
		TestLock(mutex_t* m)
		{
		}

		void run(thread_t* thread)
		{
			testMutex->lock();
			_timestamp = datetime_t::sNow();
			testMutex->unlock();
		}

		const datetime_t& timestamp() const
		{
			return _timestamp;
		}

		DCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
		mutex_t* testMutex;
		datetime_t _timestamp;
	};

	class TestTryLock: public ncore::thread_functor
	{
	public:
		TestTryLock(mutex_t* m) :testMutex(m), _locked(false)
		{
		}
		
		void run(thread_t* thread)
		{
			if (testMutex->tryLock())
			{
				_locked = true;
				testMutex->unlock();
			}
		}

		bool locked() const
		{
			return _locked;
		}

		DCORE_CLASS_PLACEMENT_NEW_DELETE

	private:
	mutex_t* testMutex;
		bool _locked;
	};
}

UNITTEST_SUITE_BEGIN(mutex)
{
    UNITTEST_FIXTURE(main)
    {
		UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testLock)
		{
			mutex_t* testMutex = threading_t::instance()->create_mutex();
			testMutex->lock();
			TestLock* tl = Allocator->construct<TestLock>(testMutex);
			thread_t* thr1 = threading_t::instance()->create_thread("test", tl, thread_t::default_stacksize(), thread_t::default_priority());
			thr1->start();
			datetime_t now = datetime_t::sNow();
			threading_t::sleep(2000);
			testMutex->unlock();
			threading_t::instance()->join(thr1);
			CHECK_TRUE (tl->timestamp() > now);
			threading_t::instance()->destroy(thr1);	
			threading_t::instance()->destroy(testMutex);	
			Allocator->destruct(tl);
		}


		UNITTEST_TEST(testTryLock)
		{
			// mutex_t* testMutex = threading_t::instance()->create_mutex();

			// thread_t thr1;
			// TestTryLock ttl1;
			// thr1.start(&ttl1);
			// threading_t::instance()->join(&thr1);
			// CHECK_TRUE (ttl1.locked());
	
			// testMutex->lock();
			// thread_t thr2;
			// TestTryLock ttl2;
			// thr2.start(&ttl2);
			// threading_t::instance()->join(&thr2);
			// testMutex->unlock();
			// CHECK_TRUE (!ttl2.locked());

			mutex_t* testMutex = threading_t::instance()->create_mutex();

			TestTryLock* tl1 = Allocator->construct<TestTryLock>(testMutex);
			thread_t* thr1 = threading_t::instance()->create_thread("test1", tl1, thread_t::default_stacksize(), thread_t::default_priority());
			thr1->start();
			threading_t::instance()->join(thr1);
			CHECK_TRUE (tl1->locked());
			
			testMutex->lock();

			TestTryLock* tl2 = Allocator->construct<TestTryLock>(testMutex);
			thread_t* thr2 = threading_t::instance()->create_thread("test2", tl2, thread_t::default_stacksize(), thread_t::default_priority());
			thr2->start();

			threading_t::instance()->join(thr2);

			testMutex->unlock();
			CHECK_TRUE (!tl2->locked());

			Allocator->destruct(tl2);
			Allocator->destruct(tl1);

			threading_t::instance()->destroy(testMutex);	

			threading_t::instance()->destroy(thr1);	
			threading_t::instance()->destroy(thr2);
		}

	}
}
UNITTEST_SUITE_END
