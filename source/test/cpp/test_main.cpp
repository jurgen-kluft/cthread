#include "xbase/x_base.h"
#include "xbase/x_console.h"
#include "xbase/x_allocator.h"
#include "xtime/x_time.h"

#include "xunittest/xunittest.h"
#include "xunittest/private/ut_ReportAssert.h"

UNITTEST_SUITE_LIST(xMThreadUnitTest);

UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xevent);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xmutex);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xsemaphore);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xthread);

namespace xcore
{
	// Our own assert handler
	class UnitTestAssertHandler : public xcore::x_asserthandler
	{
	public:
		UnitTestAssertHandler()
		{
			NumberOfAsserts = 0;
		}

		virtual xcore::xbool	HandleAssert(u32& flags, const char* fileName, s32 lineNumber, const char* exprString, const char* messageString)
		{
			UnitTest::reportAssert(exprString, fileName, lineNumber);
			NumberOfAsserts++;
			return false;
		}


		xcore::s32		NumberOfAsserts;
	};

	class UnitTestAllocator : public UnitTest::Allocator
	{
		xcore::x_iallocator*	mAllocator;
	public:
		UnitTestAllocator(xcore::x_iallocator* allocator) { mAllocator = allocator; }
		virtual void*	Allocate(xsize_t size) { return mAllocator->allocate((u32)size, sizeof(void*)); }
		virtual void	Deallocate(void* ptr) { mAllocator->deallocate(ptr); }
	};

	class TestAllocator : public x_iallocator
	{
		x_iallocator*		mAllocator;
	public:
		TestAllocator(x_iallocator* allocator) : mAllocator(allocator) { }

		virtual const char*	name() const { return "xbase unittest test heap allocator"; }

		virtual void*		allocate(xsize_t size, u32 alignment)
		{
			UnitTest::IncNumAllocations();
			return mAllocator->allocate(size, alignment);
		}

		virtual void*		reallocate(void* mem, xsize_t size, u32 alignment)
		{
			if (mem == NULL)
				return allocate(size, alignment);
			else
				return mAllocator->reallocate(mem, size, alignment);
		}

		virtual void		deallocate(void* mem)
		{
			UnitTest::DecNumAllocations();
			mAllocator->deallocate(mem);
		}

		virtual void		release()
		{
			mAllocator->release();
			mAllocator = NULL;
		}
	};
}

xcore::x_iallocator* gTestAllocator = NULL;
xcore::UnitTestAssertHandler gAssertHandler;

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
	xbase::x_Init();

#ifdef TARGET_DEBUG
	xcore::x_asserthandler::sRegisterHandler(&gAssertHandler);
#endif

	xtime::x_Init();

	xcore::x_iallocator* systemAllocator = xcore::x_iallocator::get_default();
	xcore::UnitTestAllocator unittestAllocator(systemAllocator);
	UnitTest::SetAllocator(&unittestAllocator);

	xcore::xconsole::write("Configuration: ");
	xcore::xconsole::writeLine(TARGET_FULL_DESCR_STR);

	xcore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;

	int r = UNITTEST_SUITE_RUN(reporter, xMThreadUnitTest);

	gTestAllocator->release();

	UnitTest::SetAllocator(NULL);

	xtime::x_Exit();
	xbase::x_Exit();
	return r == 0;
}

