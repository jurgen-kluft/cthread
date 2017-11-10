#include "xbase\x_target.h"
#include "xbase\x_debug.h"
#include "xmthread\x_condition.h"

namespace xcore 
{
	xcondition::xcondition()
		: _waiters(NULL)
	{
	}

	xcondition::~xcondition()
	{
	}

	void xcondition::signal()
	{
		xfastmutex::xscoped_lock lock(_mutex);
		if (_waiters!=NULL)
		{
			// Deque before trigger the event since that will result in
			// the waiter structure not being on the stack anymore.
			waiter* w = dequeue();
			w->_event.set();
		}
	}

	void xcondition::broadcast()
	{
		xfastmutex::xscoped_lock lock(_mutex);
		waiter* it = _waiters;
		waiter* end = _waiters;
		while (it != NULL)
		{
			waiter* next = it->_next;
			next = next==end ? NULL : next;
			it->_event.set();
			it = next;
		}
		_waiters = NULL;
	}

	void xcondition::enqueue(waiter* w)
	{
		// Enqueue is adding to the front
		if (_waiters != NULL)
		{
			w->_next = _waiters;
			w->_prev = _waiters->_prev;

			w->_next->_prev = w;
			w->_prev->_next = w;

			_waiters = w;
		}
		else
		{
			_waiters = w;
			_waiters->_next = w;
			_waiters->_prev = w;
		}
	}

	xcondition::waiter* xcondition::dequeue()
	{
		if (_waiters != NULL)
		{
			// Deque is removing from the back
			waiter* tail = _waiters->_prev;
			if (_waiters != tail)
			{
				// Unlink the 'tail' node
				tail->_next->_prev = tail->_prev;
				tail->_prev->_next = tail->_next;
			}
			else
			{
				_waiters = NULL;
			}
			tail->_next = NULL;
			tail->_prev = NULL;
			return tail;
		}
		return NULL;
	}

	void xcondition::dequeue(waiter& w)
	{
		if (_waiters == &w)
			_waiters = w._next;
		if (_waiters == &w)
			_waiters = NULL;

		// Unlink 'w'
		w._next->_prev = w._prev;
		w._prev->_next = w._next;

		// Clear 'w'
		w._next = 0;
		w._prev = 0;
	}


} // namespace xcore
