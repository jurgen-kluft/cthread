#include "xbase/x_base.h"
#include "xbase/x_allocator.h"
#include "xbase/x_string.h"
#include "xtime\x_time.h"

#include "xunittest\xunittest.h"

UNITTEST_SUITE_LIST(xMThreadUnitTest);

UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xauto_ptr);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xcondition);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xnevent);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xnmutex);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xrwlock);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xsemaphore);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xshared_ptr);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xthread);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xthread_ls);
UNITTEST_SUITE_DECLARE(xMThreadUnitTest, xthreadpool);


namespace xcore
{
	class UnitTestAllocator : public UnitTest::Allocator
	{
		xcore::x_iallocator*	mAllocator;
	public:
						UnitTestAllocator(xcore::x_iallocator* allocator)	{ mAllocator = allocator; }
		virtual void*	Allocate(int size)									{ return mAllocator->allocate(size, 4); }
		virtual void	Deallocate(void* ptr)								{ mAllocator->deallocate(ptr); }
	};

	class TestAllocator : public x_iallocator
	{
		x_iallocator*		mAllocator;
	public:
							TestAllocator(x_iallocator* allocator) : mAllocator(allocator) { }

		virtual const char*	name() const { return "xbase unittest test heap allocator"; }

		virtual void*		allocate(u32 size, u32 alignment)
		{
			UnitTest::IncNumAllocations();
			return mAllocator->allocate(size, alignment);
		}

		virtual void*		reallocate(void* mem, u32 size, u32 alignment)
		{
			if (mem==NULL)
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

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
	xcore::x_iallocator* systemAllocator = xcore::gCreateSystemAllocator();
	xcore::UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);

	xcore::xconsole::addDefault();

	xcore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;
	
	xcore::x_TimeInit();
	xcore::x_Init(gTestAllocator);
	int r = UNITTEST_SUITE_RUN(reporter, xMThreadUnitTest);
	xcore::x_Exit();
	xcore::x_TimeExit();

	gTestAllocator->release();
	UnitTest::SetAllocator(NULL);
	return r==0;
}

