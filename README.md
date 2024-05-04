# cthread, cross platform thread library (c++)

- platforms: windows / mac
- thread create/wait/destroy
- primitives
  - mutex
  - event
  - semaphore

```c++
static void highest_priority_thread_fn(void* arg)
{
    threading_t* threading = static_cast<threading_t>(arg);
    thread_t* current = threading->current_thread();

    // Here the high priority thread code
    bool active = true;
    while (active)
    {

        active = false;
    }
}

threading_t* threading = threading_t::create(alloc_t::get_system());

void* my_arg = threading;
thread_t* thread_main = threading->create_thread("main thread", my_arg, highest_priority_thread_fn, threading_t::HIGHEST_PRIORITY);

mutex_t* mutex = threading->create_mutex("shared resource lock");
event_t* event = threading->create_event("resource locked");

threading->wait(thread_main);
threading->destroy(thread_main);
threading->destroy(mutex);
threading->destroy(event);
threading->destroy();
```
