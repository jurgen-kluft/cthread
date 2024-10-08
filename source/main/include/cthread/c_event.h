#ifndef __CMTHREAD_EVENT_H__
#define __CMTHREAD_EVENT_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    struct event_data_t;

    // An event_t is a synchronization object that allows one thread to signal one or more
    // other threads that a certain event has happened.
    // Usually, one thread signals an event, while one or more other threads wait
    // for an event to become signalled.
    class event_t
    {
    public:
        void set();
        // Signals the event.
        // If autoReset is true, only one thread waiting for the event can resume execution.
        // If autoReset is false, all waiting threads/ can resume execution.

        void wait(); // Waits for the event to become signalled.
        void reset(); // Resets the event to unsignalled state.
        void release(); // Releases the event object back to threading

    protected:
        friend class threading_t;
        event_t()
            : m_data(nullptr)
        {
        }

        bool          init(event_data_t* data, bool autoReset = true);
        event_data_t* m_data;

    private:
        event_t(const event_t&)            = delete;
        event_t& operator=(const event_t&) = delete;
        ~event_t()                         = default;
    };

} // namespace ncore

#endif // __CMTHREAD_EVENT_H__
