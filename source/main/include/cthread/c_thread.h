#ifndef __XMTHREAD_THREAD_H__
#define __XMTHREAD_THREAD_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	class xthread_functor;

	class cthread
	{
	public:	
		typedef s32       idx_t;
		typedef u64       tid_t;

		class e_priority
        {
        public:
            inline e_priority()
                : prio(NORMAL)
            {
            }
            inline e_priority(s32 p)
                : prio(p)
            {
            }

            bool operator==(const e_priority& p) const { return prio == p.prio; }
            bool operator!=(const e_priority& p) const { return prio != p.prio; }
            bool operator<(const e_priority& p) const { return prio < p.prio; }
            bool operator<=(const e_priority& p) const { return prio <= p.prio; }
            bool operator>(const e_priority& p) const { return prio > p.prio; }
            bool operator>=(const e_priority& p) const { return prio >= p.prio; }

            s32 prio;

            static const s32 LOWEST;
            static const s32 LOW;
            static const s32 NORMAL;
            static const s32 HIGH;
            static const s32 HIGHEST;
        };

        enum e_state
        {
            STATE_CREATED = 0,
            STATE_RUNNING = 1,
            STATE_STOPPED = 2
        };

		tid_t				get_tid() const;					/// Returns the native thread ID of the thread.
		idx_t				get_idx() const;					/// Returns the unique thread ID of the thread.
		const char*			get_name() const;					/// Returns the name of the thread.
		e_priority			get_priority() const;				/// Returns the thread's priority.
		e_state				get_state() const;					/// Returns state of the thread
		u32					get_stacksize() const;

		void                set_priority(e_priority p); /// Sets the thread's priority.

		void				start();

		void				join();
		bool				join(u32 milliseconds);

		static cthread*		current();
        static tid_t        current_tid();
        static idx_t        current_idx();
		static void			sleep(u32 milliseconds);
		static void			yield();
		static void			exit();

		static u32          default_stacksize();
		static e_priority   default_priority();

	protected:
        friend class xthreading;

        cthread();                                               /// Creates a thread. Call start() to start it.
        cthread(const char* name);                               /// Creates a named thread. Call start() to start it.
        cthread(const char* name, e_priority p, u32 stack_size); /// Creates a thread using some configuration. Call start() to start it.
        cthread(const cthread&);
        cthread& operator=(const cthread&);
        ~cthread(); /// Destroys the thread.
	};

} // namespace ncore


#endif // __XMTHREAD_THREAD_H__
