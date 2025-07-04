#include "ccore/c_target.h"
#include "ccore/c_debug.h"
#include "cbase/c_allocator.h"
#include "ctime/c_time.h"

#include "cthread/c_thread.h"
#include "cthread/c_threading.h"
#include "cthread/c_semaphore.h"

#include "cunittest/cunittest.h"

using ncore::nthread::thread_t;
using ncore::nthread::thread_data_t;
using ncore::nthread::threading_t;
using ncore::nthread::sema_t;

class SemaRunnable : public ncore::nthread::thread_fn_t
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
    void quit() {}

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
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() { ncore::ntime::init(); }
        UNITTEST_FIXTURE_TEARDOWN() { ncore::ntime::exit(); }

        UNITTEST_TEST(testInitZero)
        {
            threading_t* threading = threading_t::create(Allocator);
            {
                SemaRunnable r(0, 3);
                CHECK_TRUE(r.wait());
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
                CHECK_TRUE(r.try_wait(10));

                thread_t* t = threading_t::instance()->create_thread("Thread", &r, thread_t::default_priority(), thread_t::default_stacksize());
                t->start();
                threading_t::sleep(100);
                CHECK_TRUE(!r.ran());
                r.set();
                t->join();
                CHECK_TRUE(r.ran());
                CHECK_TRUE(r.try_wait(10));
            }
            threading_t::destroy(threading);
        }

        UNITTEST_TEST(testInitNonZero)
        {
            threading_t* threading = threading_t::create(Allocator);
            {
                SemaRunnable r(2, 2);
                r.wait();
                CHECK_TRUE(r.try_wait(10));
                CHECK_TRUE(!r.try_wait(10));
                r.set();
                CHECK_TRUE(r.try_wait(10));
                CHECK_TRUE(!r.try_wait(10));
            }
            threading_t::destroy(threading);
        }
    }
}
UNITTEST_SUITE_END
