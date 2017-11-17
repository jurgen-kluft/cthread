#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_rwLock.h"
#include "xthread/x_thread.h"
#include "xthread/x_runnable.h"

#include "xtime\x_datetime.h"

#include "xunittest\xunittest.h"

using xcore::xrwlock;
using xcore::xthread;
using xcore::xrunnable;
using xcore::s32;

class RWLockRunnable: public xrunnable
{
public:
	RWLockRunnable(xrwlock& lock, volatile s32& counter)
		: _lock(lock)
		, _counter(counter)
		, _ok(true)
	{
	}
	
	void run()
	{
		s32 lastCount = 0;
		for (s32 i = 0; i < 10000; ++i)
		{
			_lock.readLock();
			lastCount = _counter;
			for (s32 k = 0; k < 100; ++k)
			{
				if (_counter != lastCount) 
					_ok = false;
				xthread::yield();
			}
			_lock.unlock();
			_lock.writeLock();
			for (s32 k = 0; k < 100; ++k)
			{
				--_counter;
				xthread::yield();
			}
			for (s32 k = 0; k < 100; ++k)
			{
				++_counter;
				xthread::yield();
			}
			++_counter;
			if (_counter <= lastCount)
				_ok = false;
			_lock.unlock();
		}
	}
	
	bool ok() const
	{
		return _ok;
	}
	
private:
	xrwlock& _lock;
	volatile s32& _counter;
	bool _ok;
};


class RWTryLockRunnable: public xrunnable
{
public:
	RWTryLockRunnable(xrwlock& lock, volatile s32& counter)
		: _lock(lock)
		, _counter(counter)
		, _ok(true)
	{
	}
	
	void run()
	{
		s32 lastCount = 0;
		for (s32 i = 0; i < 10000; ++i)
		{
			while (!_lock.tryReadLock()) xthread::yield();
			lastCount = _counter;
			for (s32 k = 0; k < 100; ++k)
			{
				if (_counter != lastCount) _ok = false;
				xthread::yield();
			}
			_lock.unlock();
			while (!_lock.tryWriteLock()) xthread::yield();
			for (s32 k = 0; k < 100; ++k)
			{
				--_counter;
				xthread::yield();
			}
			for (s32 k = 0; k < 100; ++k)
			{
				++_counter;
				xthread::yield();
			}
			++_counter;
			if (_counter <= lastCount) _ok = false;
			_lock.unlock();
		}
	}
	
	bool ok() const
	{
		return _ok;
	}
	
private:
	xrwlock&		_lock;
	volatile s32&	_counter;
	bool			_ok;
};

UNITTEST_SUITE_BEGIN(xrwlock)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testLock)
		{
			xrwlock lock;
			s32 counter = 0;
			RWLockRunnable r1(lock, counter);
			RWLockRunnable r2(lock, counter);
			RWLockRunnable r3(lock, counter);
			RWLockRunnable r4(lock, counter);
			RWLockRunnable r5(lock, counter);
			xthread t1;
			xthread t2;
			xthread t3;
			xthread t4;
			xthread t5;
			t1.start(r1);
			t2.start(r2);
			t3.start(r3);
			t4.start(r4);
			t5.start(r5);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			CHECK_TRUE (counter == 50000);
			CHECK_TRUE (r1.ok());
			CHECK_TRUE (r2.ok());
			CHECK_TRUE (r3.ok());
			CHECK_TRUE (r4.ok());
			CHECK_TRUE (r5.ok());
		}

		UNITTEST_TEST(testTryLock)
		{
			xrwlock lock;
			s32 counter = 0;
			RWTryLockRunnable r1(lock, counter);
			RWTryLockRunnable r2(lock, counter);
			RWTryLockRunnable r3(lock, counter);
			RWTryLockRunnable r4(lock, counter);
			RWTryLockRunnable r5(lock, counter);
			xthread t1;
			xthread t2;
			xthread t3;
			xthread t4;
			xthread t5;
			t1.start(r1);
			t2.start(r2);
			t3.start(r3);
			t4.start(r4);
			t5.start(r5);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			CHECK_TRUE (counter == 50000);
			CHECK_TRUE (r1.ok());
			CHECK_TRUE (r2.ok());
			CHECK_TRUE (r3.ok());
			CHECK_TRUE (r4.ok());
			CHECK_TRUE (r5.ok());
		}

	}
}
UNITTEST_SUITE_END
