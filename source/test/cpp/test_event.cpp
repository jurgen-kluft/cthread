#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"
#include "ctime/c_time.h"

#include "cthread/c_event.h"
#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "ctime/c_datetime.h"

#include "cunittest/cunittest.h"

using namespace ncore;
using namespace ncore::nthread;
using ncore::datetime_t;

namespace
{
    class TestEvent : public ncore::nthread::thread_fn_t
    {
    public:
		TestEvent(ncore::nthread::event_t* data)
		{
			testEvent = data;
		}

        void start(nthread::thread_t* t, nthread::thread_data_t* d)
        {

        }

        void run()
        {
            testEvent->wait();
            _timestamp = datetime_t::sNow();
        }

        void exit()
        {

        }

        void quit()
        {
            // mark a variable to indicate that the run function should exit
        }

        const datetime_t& timestamp() const { return _timestamp; }

    private:
        ncore::nthread::event_t*   testEvent;
        ncore::datetime_t _timestamp;
    };
} // namespace

UNITTEST_SUITE_BEGIN(event)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() { ntime::init();  }
        UNITTEST_FIXTURE_TEARDOWN() { ntime::exit(); }

        UNITTEST_TEST(testNamedEvent)
        {
            threading_t* threading = threading_t::create(Allocator);
			event_t* testEvent = threading->create_event("test", false);

            TestEvent te(testEvent);
            thread_t* thr1 = threading->create_thread("test1", &te, thread_t::default_priority(), thread_t::default_stacksize());
            thr1->start();
            datetime_t now = datetime_t::sNow();
            threading_t::sleep(2000);
            testEvent->set();
            threading_t::instance()->join(thr1);
            CHECK_TRUE(te.timestamp() >= now);

            thread_t* thr2 = threading->create_thread("test2", &te, thread_t::default_priority(), thread_t::default_stacksize());
            thr2->start();
            now = datetime_t::sNow();
            threading_t::sleep(2000);
            testEvent->set();
            threading_t::instance()->join(thr2);
            CHECK_TRUE(te.timestamp() >= now);

            threading_t::destroy(threading);
        }
    }
}
UNITTEST_SUITE_END
