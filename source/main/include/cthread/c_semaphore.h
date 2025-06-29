#ifndef __CMTHREAD_SEMAPHORE_H__
#define __CMTHREAD_SEMAPHORE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nthread
    {
        struct sema_data_t;

        // A sema_t is a synchronization object with the following
        // characteristics:
        // A semaphore has a value that is constrained to be a non-negative
        // integer and two atomic operations. The allowable operations are V
        // (here called set()) and P (here called wait()). A V (set()) operation
        // increases the value of the semaphore by one.
        // A P (wait()) operation decreases the value of the semaphore by one,
        // provided that can be done without violating the constraint that the
        // value be non-negative. A P (wait()) operation that is initiated when
        // the value of the semaphore is 0 suspends the calling thread.
        // The calling thread may continue when the value becomes positive again.
        class sema_t
        {
        public:
            void signal();
            // Increments the semaphore's value by one and
            // thus signals the semaphore. Another thread
            // waiting for the semaphore will be able
            // to continue.

            void wait();
            // Waits for the semaphore to become signalled.
            // To become signalled, a semaphore's value must
            // be greater than zero.
            // Decrements the semaphore's value by one.

            bool try_wait(u32 milliseconds);
            // Waits for the semaphore to become signalled.
            // To become signalled, a semaphore's value must
            // be greater than zero.
            // Decrements the semaphore's value by one.
            // Returns false if there is a time-out and true
            // if we got a signal.

            void release();
            // Releases the semaphore object back to threading

        protected:
            friend class threading_t;
            sema_t()
                : m_data(nullptr)
            {
            }

            // Initializes the semaphore. The current value of the semaphore is given in n.
            // The maximum value of the semaphore is given in max.
            // If only n is given, it must be greater than zero.
            // If both n and max are given, max must be greater than zero, n must be greater
            // than or equal to zero and less than or equal to max.
            bool init(sema_data_t* data, s32 n);
            bool init(sema_data_t* data, s32 n, s32 max);

            sema_data_t* m_data;

        private:
            sema_t(const sema_t&);
            sema_t& operator=(const sema_t&) { return *this; }
        };
    } // namespace nthread
} // namespace ncore

#endif // __CMTHREAD_SEMAPHORE_H__
