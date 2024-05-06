#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"

#include "cthread/c_thread.h"
#include "cthread/c_threading.h"

#include "cthread/c_event.h"

#include "ctime/c_datetime.h"
#include "ctime/c_timespan.h"

#include "cunittest/cunittest.h"
#include "cthread/test_allocator.h"

using ncore::alloc_t;
using ncore::thread_t;
using ncore::thread_data_t;
using ncore::threading_t;
using ncore::event_t;
using ncore::datetime_t;
using ncore::timespan_t;
using ncore::s32;

class MyRunnable : public ncore::thread_functor
{
public:
    MyRunnable(event_t* e)
        : _ran(false)
        , _threadName(nullptr)
        , _event(e)
    {
    }

    thread_t* pThread;

    void start(thread_t* t, thread_data_t* d) { pThread = t; }

    void run()
    {
        if (pThread)
            _threadName = pThread->get_name();
        _ran = true;
        _event->wait();
    }

    void exit() {}

    bool        ran() const { return _ran; }
    const char* threadName() const { return _threadName; }
    void        notify() { _event->set(); }
    static void staticFunc() { ++_staticVar; }
    static s32  _staticVar;

private:
    bool        _ran;
    const char* _threadName;
    event_t*    _event;
};

s32 MyRunnable::_staticVar = 0;

void freeFunc() { ++MyRunnable::_staticVar; }
void freeFunc(void* pData) { MyRunnable::_staticVar += *reinterpret_cast<s32*>(pData); }

UNITTEST_SUITE_BEGIN(thread)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(testThread)
        {
            event_t* e = threading_t::instance()->create_event("event", false);
            MyRunnable r(e);
            thread_t*   thread = threading_t::instance()->create_thread("Thread", &r, thread_t::default_stacksize(), thread_t::default_priority());

            CHECK_TRUE(!thread->is_running());
            thread->start();
            threading_t::sleep(200);
            CHECK_TRUE(thread->is_running());
            r.notify();
            threading_t::instance()->join(thread);
            CHECK_TRUE(!thread->is_running());
            CHECK_TRUE(r.ran());
            CHECK_NOT_NULL(r.threadName());
            CHECK_FALSE(*r.threadName() == '\0');

            threading_t::instance()->destroy(e);
            threading_t::instance()->destroy(thread);
        }

        UNITTEST_TEST(testNamedThread)
        {
            event_t* e = threading_t::instance()->create_event("event", false);
            MyRunnable r(e);
            thread_t* thread = threading_t::instance()->create_thread("MyThread", &r, thread_t::default_stacksize(), thread_t::default_priority());

            thread->start();
            r.notify();
            thread->join();
            CHECK_TRUE(r.ran());
            CHECK_EQUAL(r.threadName(), "MyThread");

            threading_t::instance()->destroy(e);
            threading_t::instance()->destroy(thread);
        }

        UNITTEST_TEST(testCurrent) { CHECK_NULL(threading_t::current()); }

        UNITTEST_TEST(testThreads)
        {
            event_t* e1 = threading_t::instance()->create_event("event1", false);
            event_t* e2 = threading_t::instance()->create_event("event2", false);
            event_t* e3 = threading_t::instance()->create_event("event3", false);
            event_t* e4 = threading_t::instance()->create_event("event4", false);

            MyRunnable r1(e1);
            MyRunnable r2(e2);
            MyRunnable r3(e3);
            MyRunnable r4(e4);

            thread_t* thread1 = threading_t::instance()->create_thread("Thread1", &r1, thread_t::default_stacksize(), thread_t::default_priority());
            thread_t* thread2 = threading_t::instance()->create_thread("Thread2", &r2, thread_t::default_stacksize(), thread_t::default_priority());
            thread_t* thread3 = threading_t::instance()->create_thread("Thread3", &r3, thread_t::default_stacksize(), thread_t::default_priority());
            thread_t* thread4 = threading_t::instance()->create_thread("Thread4", &r4, thread_t::default_stacksize(), thread_t::default_priority());

            CHECK_TRUE(!thread1->is_running());
            CHECK_TRUE(!thread2->is_running());
            CHECK_TRUE(!thread3->is_running());
            CHECK_TRUE(!thread4->is_running());
            thread1->start();
            threading_t::sleep(200);
            CHECK_TRUE(thread1->is_running());
            CHECK_TRUE(!thread2->is_running());
            CHECK_TRUE(!thread3->is_running());
            CHECK_TRUE(!thread4->is_running());
            thread2->start();
            thread3->start();
            thread4->start();
            threading_t::sleep(200);
            CHECK_TRUE(thread1->is_running());
            CHECK_TRUE(thread2->is_running());
            CHECK_TRUE(thread3->is_running());
            CHECK_TRUE(thread4->is_running());
            r4.notify();
            threading_t::instance()->join(thread4);
            CHECK_TRUE(!thread4->is_running());
            CHECK_TRUE(thread1->is_running());
            CHECK_TRUE(thread2->is_running());
            CHECK_TRUE(thread3->is_running());
            r3.notify();
            threading_t::instance()->join(thread3);
            CHECK_TRUE(!thread3->is_running());
            r2.notify();
            threading_t::instance()->join(thread2);
            CHECK_TRUE(!thread2->is_running());
            r1.notify();
            threading_t::instance()->join(thread1);
            CHECK_TRUE(!thread1->is_running());

            CHECK_TRUE(r1.ran());
            CHECK_EQUAL(r1.threadName(), "Thread1");

            CHECK_TRUE(r2.ran());
            CHECK_EQUAL(r2.threadName(), "Thread2");

            CHECK_TRUE(r3.ran());
            CHECK_EQUAL(r3.threadName(), "Thread3");

            CHECK_TRUE(r4.ran());
            CHECK_EQUAL(r4.threadName(), "Thread4");

            threading_t::instance()->destroy(e1);
            threading_t::instance()->destroy(e2);
            threading_t::instance()->destroy(e3);
            threading_t::instance()->destroy(e4);

            threading_t::instance()->destroy(thread1);
            threading_t::instance()->destroy(thread2);
            threading_t::instance()->destroy(thread3);
            threading_t::instance()->destroy(thread4);
        }

        UNITTEST_TEST(testJoin)
        {
            event_t* e = threading_t::instance()->create_event("event", false);
            MyRunnable r(e);
            thread_t*  thread = threading_t::instance()->create_thread("Thread", &r, thread_t::default_stacksize(), thread_t::default_priority());

            CHECK_TRUE(!thread->is_running());
            thread->start();
            threading_t::sleep(200);
            CHECK_TRUE(thread->is_running());
            CHECK_TRUE(!thread->join(100));
            r.notify();
            CHECK_TRUE(thread->join(500));
            CHECK_TRUE(!thread->is_running());

            threading_t::instance()->destroy(e);
            threading_t::instance()->destroy(thread);
        }

        UNITTEST_TEST(testThreadFunction)
        {
            event_t* e = threading_t::instance()->create_event("event", false);
            MyRunnable f(e);
            thread_t* thread = threading_t::instance()->create_thread("Thread", &f, thread_t::default_stacksize(), thread_t::default_priority());

            CHECK_TRUE(!thread->is_running());

            s32 tmp = MyRunnable::_staticVar;
            thread->start();
            threading_t::instance()->join(thread);
            CHECK_TRUE(tmp * 2 == MyRunnable::_staticVar);

            CHECK_TRUE(!thread->is_running());

            tmp = MyRunnable::_staticVar = 0;
            thread->start();
            threading_t::instance()->join(thread);
            CHECK_TRUE(0 == MyRunnable::_staticVar);

            threading_t::instance()->destroy(e);
            threading_t::instance()->destroy(thread);
        }

        UNITTEST_TEST(testSleep)
        {
            datetime_t start = datetime_t::sNow();
            threading_t::sleep(200);
            datetime_t end     = datetime_t::sNow();
            timespan_t elapsed = end - start;
            CHECK_TRUE(elapsed.totalMilliseconds() >= 190 && elapsed.totalMilliseconds() < 250);
        }
    }
}
UNITTEST_SUITE_END
