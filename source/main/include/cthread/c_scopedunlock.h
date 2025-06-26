#ifndef __CMTHREAD_SCOPEDUNLOCK_H__
#define __CMTHREAD_SCOPEDUNLOCK_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nthread
    {
        // A class that simplifies thread synchronization with a mutex.
        // The constructor accepts a mutex and unlocks it.
        // The destructor locks the mutex.
        template <class M> class unlock_t
        {
        public:
            inline unlock_t(M* mutex, bool unlockNow = true)
                : _mutex(mutex)
            {
                if (unlockNow)
                    _mutex->unlock();
            }

            inline ~unlock_t() { _mutex->lock(); }

        private:
            M* _mutex;

            unlock_t();
            unlock_t(const unlock_t&);

            unlock_t& operator=(const unlock_t&);
        };
    } // namespace nthread
} // namespace ncore

#endif // __CMTHREAD_SCOPEDUNLOCK_H__
