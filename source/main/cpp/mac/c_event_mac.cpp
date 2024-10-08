#include "ccore/c_target.h"
#include "ccore/c_debug.h"

#ifdef TARGET_MAC
#    include "cthread/c_event.h"
#    include "cthread/private/c_thread_mac.h"

namespace ncore
{
    // -------------------------------------------------------------------------------
    // Platform specific semaphore part
    static void s_sema_init(semaphore_t& sema, s32 initialCount = 0)
    {
        ASSERT(initialCount >= 0);
        ::semaphore_create(mach_task_self(), &sema, SYNC_POLICY_FIFO, initialCount);
    }

    static void s_sema_destroy(semaphore_t& sema) { ::semaphore_destroy(mach_task_self(), sema); }
    static void s_sema_wait(semaphore_t& sema) { ::semaphore_wait(sema); }
    static void s_sema_signal(semaphore_t& sema) { ::semaphore_signal(sema); }
    static void s_sema_signal(semaphore_t& sema, s32 count)
    {
        while (count-- > 0)
        {
            ::semaphore_signal(sema);
        }
    }

    // -------------------------------------------------------------------------------
    // Lightweight semaphore part
    static void s_lightweight_sema_init(event_data_t* data)
    {
        s_sema_init(data->m_sema, 0);
        data->m_count = 0;
    }

    static void s_lightweight_sema_destroy(event_data_t* data) { s_sema_destroy(data->m_sema); }

    static void s_lightweight_sema_waitWithPartialSpinning(event_data_t* data)
    {
        s32 oldCount;

        // Is there a better way to set the initial spin count?
        s32 spin = 10000;
        while (spin--)
        {
            oldCount = data->m_count.load(std::memory_order_relaxed);
            if ((oldCount > 0) && data->m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire))
                return;
            std::atomic_signal_fence(std::memory_order_acquire); // Prevent the compiler from collapsing the loop.
        }
        oldCount = data->m_count.fetch_sub(1, std::memory_order_acquire);
        if (oldCount <= 0)
        {
            s_sema_wait(data->m_sema);
        }
    }

    static bool s_lightweight_sema_tryWait(event_data_t* data)
    {
        s32 oldCount = data->m_count.load(std::memory_order_relaxed);
        return (oldCount > 0 && data->m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire));
    }

    void s_lightweight_sema_wait(event_data_t* data)
    {
        if (!s_lightweight_sema_tryWait(data))
            s_lightweight_sema_waitWithPartialSpinning(data);
    }

    static void s_lightweight_sema_signal(event_data_t* data, s32 count = 1)
    {
        s32 oldCount  = data->m_count.fetch_add(count, std::memory_order_release);
        s32 toRelease = -oldCount < count ? -oldCount : count;
        if (toRelease > 0)
        {
            s_sema_signal(data->m_sema, toRelease);
        }
    }

    // -------------------------------------------------------------------------------
    // Event implementation using lightweight semaphore
    bool event_t::init(event_data_t* data, bool autoReset)
    {
        m_data = data;
        s_lightweight_sema_init(m_data);
        m_data->m_status = 0;
        return true;
    }

    void event_t::release()
    {
        s_lightweight_sema_destroy(m_data);
        threading_t::instance()->destroy(this);
    }

    void event_t::set()
    {
        s32 oldStatus = m_data->m_status.load(std::memory_order_relaxed);
        for (;;) // Increment atomically via CAS loop.
        {
            ASSERT(oldStatus <= 1);
            s32 const newStatus = oldStatus < 1 ? oldStatus + 1 : 1;
            if (m_data->m_status.compare_exchange_weak(oldStatus, newStatus, std::memory_order_release, std::memory_order_relaxed))
                break;
            // The compare-exchange failed, likely because another thread changed m_status.
            // oldStatus has been updated. Retry the CAS loop.
        }
        if (oldStatus < 0)
            s_lightweight_sema_signal(m_data); // Release one waiting thread.
    }

    void event_t::wait()
    {
        s32 oldStatus = m_data->m_status.fetch_sub(1, std::memory_order_acquire);
        ASSERT(oldStatus <= 1);
        if (oldStatus < 1)
        {
            s_lightweight_sema_wait(m_data);
        }
    }

} // namespace ncore

#endif
