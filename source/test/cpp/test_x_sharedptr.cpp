#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xbase\x_allocator.h"

#include "xmthread\x_sharedptr.h"

#include "xunittest\xunittest.h"

using xcore::xshared_ptr;
using xcore::s32;

namespace
{
	class TestObject
	{
	public:
		TestObject(const char* data)
			: _data(data)
		{
			++_count;
		}
		
		virtual ~TestObject()
		{
			--_count;
		}
		
		const char* data()
		{
			return _data;
		}
		
		static s32 count()
		{
			return _count;
		}
		
	private:
		const char*	_data;
		static s32 _count;
	};
	
	s32 TestObject::_count = 0;

	class DerivedObject: public TestObject
	{
	public:
		DerivedObject(const char* s, s32 i)
			: TestObject(s)
			, _number(i)
		{
		}

		s32 number() const
		{
			return _number;
		}
		
	private:
		s32 _number;
	};
}


UNITTEST_SUITE_BEGIN(xshared_ptr)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(testSharedPtr)
		{
			xshared_ptr<TestObject> ptr1;
			CHECK_TRUE(ptr1.get() == NULL);
			TestObject* pTO1 = new TestObject("one");
			TestObject* pTO2 = new TestObject("two");
			if (pTO2 < pTO1)
			{
				TestObject* pTmp = pTO1;
				pTO1 = pTO2;
				pTO2 = pTmp;
			}
			ASSERT(pTO1 < pTO2);
			ptr1 = pTO1;
			CHECK_TRUE(ptr1.referenceCount() == 1);
			xshared_ptr<TestObject> ptr2 = pTO2;
			xshared_ptr<TestObject> ptr3 = ptr1;
			CHECK_TRUE (ptr1.referenceCount() == 2);
			xshared_ptr<TestObject> ptr4;
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
	
			ptr1.swap(ptr2);
			CHECK_TRUE (ptr2 < ptr1);
			ptr2.swap(ptr1);

			CHECK_TRUE ((ptr1->data() == "one" && ptr2->data() == "two") || (ptr1->data() == "two" && ptr2->data() == "one"));
	
			{
				//this will assert
				//CHECK_TRUE (ptr4->data() == "four");
			}
	
			CHECK_TRUE (!(ptr4 == ptr1));
			CHECK_TRUE (!(ptr4 == ptr2));
			CHECK_TRUE (ptr4 != ptr1);
			CHECK_TRUE (ptr4 != ptr2);
	
			ptr4 = ptr2;
			CHECK_TRUE (ptr4 == ptr2);
			CHECK_TRUE (!(ptr4 != ptr2));
	
			CHECK_TRUE (TestObject::count() == 2);
			ptr1 = 0;
			ptr2 = 0;
			ptr3 = 0;
			ptr4 = 0;
			CHECK_TRUE (TestObject::count() == 0);
	
			{
				xshared_ptr<TestObject> ptr = new TestObject("");
				CHECK_TRUE (TestObject::count() == 1);
			}
			CHECK_TRUE (TestObject::count() == 0);
		}


		UNITTEST_TEST(testImplicitCast)
		{
			{
				// null assign test
				xshared_ptr<DerivedObject> ptr2;
				CHECK_NULL(ptr2.get());
				xshared_ptr<TestObject> ptr1 = ptr2;
			}
			{
				xshared_ptr<DerivedObject> ptr2(new DerivedObject("test", 666));
				CHECK_TRUE (TestObject::count() == 1);
				xshared_ptr<TestObject> ptr1 = ptr2;
				CHECK_TRUE (TestObject::count() == 1);
			}
			CHECK_TRUE (TestObject::count() == 0);
			xshared_ptr<TestObject> ptr1 = new DerivedObject("test", 666);
			CHECK_TRUE (TestObject::count() == 1);
			ptr1 = 0;
			CHECK_TRUE (TestObject::count() == 0);
		}

		UNITTEST_TEST(testExplicitCast)
		{
			xshared_ptr<TestObject> ptr1 = new DerivedObject("test", 666);
			xshared_ptr<DerivedObject> ptr2 = ptr1.unsafeCast<DerivedObject>();
			CHECK_TRUE (ptr2.get() != 0);

			ptr1 = new TestObject("test");
			CHECK_TRUE (TestObject::count() == 2);
		}

	}
}
UNITTEST_SUITE_END
