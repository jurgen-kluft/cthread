#ifndef __CMTHREAD_MUTEX_H__
#define __CMTHREAD_MUTEX_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nthread
    {
        struct mutex_data_t;

        // A mutex_t (mutual exclusion) is a synchronization
        // mechanism used to control access to a shared resource
        // in a concurrent (multithreaded) scenario.
        // Mutexes are recursive, that is, the same mutex can be
        // locked multiple times by the same thread (but, of course,
        // not by other threads).
        // Using the xscoped_lock class is the preferred way to automatically
        // lock and unlock a mutex.
        class mutex_t
        {
        public:
            void lock();
            // Locks the mutex. Blocks if the mutex
            // is held by another thread.

            bool tryLock();
            // Tries to lock the mutex. Returns false immediately
            // if the mutex is already held by another thread.
            // Returns true if the mutex was successfully locked.

            void unlock();
            // Unlocks the mutex so that it can be acquired by
            // other threads.

            void release();
            // Releases the mutex object back to threading

        protected:
            friend class threading_t;
            mutex_t()
                : m_data(nullptr)
            {
            }
            bool          init(mutex_data_t* data);
            mutex_data_t* m_data;

        private:
            ~mutex_t();
            mutex_t(const mutex_t&) {}
            mutex_t& operator=(const mutex_t&) { return *this; }
        };
    } // namespace nthread
} // namespace ncore

#endif // __CMTHREAD_MUTEX_H__
