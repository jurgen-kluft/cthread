#ifndef __CMTHREAD_TYPES_H__
#define __CMTHREAD_TYPES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
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
            STOPPED = 2
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
            LOWEST  = 0,
            LOW     = 1,
            NORMAL  = 2,
            HIGH    = 3,
            HIGHEST = 4
        };

        bool operator==(const thread_priority_t& p) const { return prio == p.prio; }
        bool operator!=(const thread_priority_t& p) const { return prio != p.prio; }
        bool operator<(const thread_priority_t& p) const { return prio < p.prio; }
        bool operator<=(const thread_priority_t& p) const { return prio <= p.prio; }
        bool operator>(const thread_priority_t& p) const { return prio > p.prio; }
        bool operator>=(const thread_priority_t& p) const { return prio >= p.prio; }

        s32 prio;
    };

} // namespace ncore

#endif