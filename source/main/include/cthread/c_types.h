#ifndef __CTHREAD_TYPES_H__
#define __CTHREAD_TYPES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nthread
    {
        typedef s32 thread_idx_t;
        typedef u64 thread_id_t;

        class thread_state_t
        {
        public:
            enum
            {
                CREATED = 0,
                RUNNING = 1,
                STOPPED = 3
            };

            inline thread_state_t()
                : state(CREATED)
            {
            }

            inline thread_state_t(s32 s)
                : state(s)
            {
            }

            bool operator==(const thread_state_t& s) const { return state == s.state; }
            bool operator!=(const thread_state_t& s) const { return state != s.state; }
            bool operator<(const thread_state_t& s) const { return state < s.state; }
            bool operator<=(const thread_state_t& s) const { return state <= s.state; }
            bool operator>(const thread_state_t& s) const { return state > s.state; }
            bool operator>=(const thread_state_t& s) const { return state >= s.state; }

            s32 state;
        };

        class thread_priority_t
        {
        public:
            inline thread_priority_t()
                : prio(NORMAL)
            {
            }
            inline thread_priority_t(s32 p)
                : prio(p)
            {
            }
            enum
            {
                IDLE         = 0,
                LOW          = 1,
                BELOW_NORMAL = 2,
                NORMAL       = 3,
                ABOVE_NORMAL = 4,
                HIGH         = 5,
                CRITICAL     = 6,
                COUNT        = 7
            };

            bool operator==(const thread_priority_t& p) const { return prio == p.prio; }
            bool operator!=(const thread_priority_t& p) const { return prio != p.prio; }
            bool operator<(const thread_priority_t& p) const { return prio < p.prio; }
            bool operator<=(const thread_priority_t& p) const { return prio <= p.prio; }
            bool operator>(const thread_priority_t& p) const { return prio > p.prio; }
            bool operator>=(const thread_priority_t& p) const { return prio >= p.prio; }

            s32 prio;
        };
    } // namespace nthread
} // namespace ncore

#endif // __CTHREAD_TYPES_H__
