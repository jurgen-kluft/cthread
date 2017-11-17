#include "xbase/x_target.h"
#include "xbase/x_debug.h"
#include "xbase/x_allocator.h"

#include "xthread/x_autoptr.h"

#include "xunittest\xunittest.h"

using xcore::xauto_ptr;
using xcore::s32;

extern xcore::x_iallocator* gTestAllocator;

namespace
{
	class TestObj
	{
	public:
		TestObj(): _rc(1)
		{
			++_count;
		}
				
		void duplicate()
		{
			++_rc;
		}
		
		void release()
		{
			if (--_rc == 0)
				delete this;
		}
		
		s32 rc() const
		{
			return _rc;
		}
		
		static s32 count()
		{
			return _count;
		}

		static xcore::x_iallocator*	sGetAllocator() { return gTestAllocator; }
		XCORE_CLASS_NEW_DELETE(sGetAllocator, 4)

	protected:
		~TestObj()
		{
			--_count;
		}
		
	private:
		s32 _rc;
		static s32 _count;
	};
	
	s32 TestObj::_count = 0;
}

UNITTEST_SUITE_BEGIN(xauto_ptr)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testAutoPtr)
		{
			{
				xauto_ptr<TestObj> ptr = new TestObj;
				CHECK_TRUE (ptr->rc() == 1);
				xauto_ptr<TestObj> ptr2 = ptr;
				CHECK_TRUE (ptr->rc() == 2);
				ptr2 = new TestObj;
				CHECK_TRUE (ptr->rc() == 1);
				xauto_ptr<TestObj> ptr3;
				ptr3 = ptr2;
				CHECK_TRUE (ptr2->rc() == 2);
				ptr3 = new TestObj;
				CHECK_TRUE (ptr2->rc() == 1);
				ptr3 = ptr2;
				CHECK_TRUE (ptr2->rc() == 2);
				CHECK_TRUE (TestObj::count() > 0);
			}
			CHECK_TRUE (TestObj::count() == 0);
		}


		UNITTEST_TEST(testOps)
		{
			xauto_ptr<TestObj> ptr1;
			CHECK_NULL(ptr1.get());

			TestObj* pTO1 = new TestObj;
			TestObj* pTO2 = new TestObj;
			if (pTO2 < pTO1)
			{
				TestObj* pTmp = pTO1;
				pTO1 = pTO2;
				pTO2 = pTmp;
			}
			CHECK_TRUE (pTO1 < pTO2);
			ptr1 = pTO1;
			xauto_ptr<TestObj> ptr2 = pTO2;
			xauto_ptr<TestObj> ptr3 = ptr1;
			xauto_ptr<TestObj> ptr4;
			CHECK_TRUE (ptr1.get() == pTO1);
			CHECK_TRUE (ptr1 == pTO1);
			CHECK_TRUE (ptr2.get() == pTO2);
			CHECK_TRUE (ptr2 == pTO2);
			CHECK_TRUE (ptr3.get() == pTO1);
			CHECK_TRUE (ptr3 == pTO1);
	
			CHECK_TRUE (ptr1 == pTO1);
			CHECK_TRUE (ptr1 != pTO2);
			CHECK_TRUE (ptr1 < pTO2);
			CHECK_TRUE (ptr1 <= pTO2);
			CHECK_TRUE (ptr2 > pTO1);
			CHECK_TRUE (ptr2 >= pTO1);
	
			CHECK_TRUE (ptr1 == ptr3);
			CHECK_TRUE (ptr1 != ptr2);
			CHECK_TRUE (ptr1 < ptr2);
			CHECK_TRUE (ptr1 <= ptr2);
			CHECK_TRUE (ptr2 > ptr1);
			CHECK_TRUE (ptr2 >= ptr1);
	
			ptr1 = pTO1;
			ptr2 = pTO2;
			ptr1.swap(ptr2);
			CHECK_TRUE (ptr2.get() == pTO1);
			CHECK_TRUE (ptr1.get() == pTO2);
		
			CHECK_TRUE (!(ptr4 == ptr1));
			CHECK_TRUE (!(ptr4 == ptr2));
			CHECK_TRUE (ptr4 != ptr1);
			CHECK_TRUE (ptr4 != ptr2);
	
			ptr4 = ptr2;
			CHECK_TRUE (ptr4 == ptr2);
			CHECK_TRUE (!(ptr4 != ptr2));
	
			CHECK_TRUE (!(!ptr1));
			ptr1 = 0;
			CHECK_TRUE (!ptr1);
		}

	}
}
UNITTEST_SUITE_END
