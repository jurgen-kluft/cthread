#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"

#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "cthread/c_semaphore.h"

#include "cunittest/cunittest.h"
#include "cthread/test_allocator.h"

using ncore::thread_t;
using ncore::thread_data_t;
using ncore::threading_t;
using ncore::sema_t;

class SemaRunnable : public ncore::thread_fn_t
{
public:
    SemaRunnable(int n, int max)
        : _ran(false)
    {
        _sema = threading_t::instance()->create_sema(n, max);
    }

    ~SemaRunnable() { threading_t::instance()->destroy(_sema); }

    void start(thread_t* t, thread_data_t* d) {}

    void run()
    {
        _sema->wait();
        _ran = true;
    }

    void exit() {}

    bool ran() const { return _ran; }
    void set() { _sema->signal(); }

    bool wait()
    {
        _sema->wait();
        return true;
    }

    bool try_wait(ncore::u32 ms) { return _sema->try_wait(ms); }

private:
    bool    _ran;
    sema_t* _sema;
};

UNITTEST_SUITE_BEGIN(semaphore)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(testInitZero)
        {
            SemaRunnable r(0, 3);
            CHECK_TRUE(!r.wait());
            r.set();
            r.wait();
            {
                r.try_wait(100);
                // will fail
            }

            r.set();
            r.set();
            CHECK_TRUE(r.try_wait(0));
            r.wait();
            CHECK_TRUE(!r.try_wait(10));

            thread_t* t = threading_t::instance()->create_thread("Thread", &r, thread_t::default_stacksize(), thread_t::default_priority());
            t->start();
            threading_t::sleep(100);
            CHECK_TRUE(!r.ran());
            r.set();
            t->join();
            CHECK_TRUE(r.ran());
            CHECK_TRUE(!r.try_wait(10));
        }

        UNITTEST_TEST(testInitNonZero)
        {
            SemaRunnable r(2, 2);
            r.wait();
            CHECK_TRUE(r.try_wait(10));
            CHECK_TRUE(!r.try_wait(10));
            r.set();
            CHECK_TRUE(r.try_wait(10));
            CHECK_TRUE(!r.try_wait(10));
        }
    }
}
UNITTEST_SUITE_END
