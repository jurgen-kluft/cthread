# cthread, cross platform thread library (c++)

- platforms: windows / mac
- thread create/wait/destroy
- primitives
  - mutex
  - event
  - semaphore

```c++
class highest_priority_thread_fn : public thread_functor
{
public:
  void run(thread_t* thread)
  {
      thread_t* current = thread;

      // Here the high priority thread code
      bool active = true;
      while (active)
      {

          active = false;
      }
  }
};

threading_t* threading = threading_t::create(alloc_t::get_system());

thread_t* thread_main = threading->create_thread("main thread", highest_priority_thread_fn, threading_t::HIGHEST_PRIORITY);

mutex_t* mutex = threading->create_mutex("shared resource lock");
event_t* event = threading->create_event("resource locked");

threading->wait(thread_main);
threading->destroy(thread_main);
threading->destroy(mutex);
threading->destroy(event);
threading->destroy();

threading::destroy(threading);
```
