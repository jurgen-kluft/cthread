#ifndef __CMTHREAD_SCOPEDLOCK_H__
#define __CMTHREAD_SCOPEDLOCK_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nthread
    {
        // A class that simplifies thread synchronization with a mutex.
        // The constructor accepts a mutex_t (and optionally a timeout value in milliseconds) and locks it.
        // The destructor unlocks the mutex.
        template <class M> class lock_t
        {
        public:
            explicit lock_t(M* primitive)
                : _primitive(primitive)
            {
                _primitive->lock();
            }
            lock_t(M& _primitive, u32 milliseconds)
                : _primitive(primitive)
            {
                _primitive->lock(milliseconds);
            }
            ~lock_t() { _primitive->unlock(); }

        private:
            M* _primitive;

            lock_t();
            lock_t(const lock_t&);

            lock_t& operator=(const lock_t&);
        };
    } // namespace nthread
} // namespace ncore

#endif // __CMTHREAD_SCOPEDLOCK_H__
