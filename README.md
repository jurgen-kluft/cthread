# xthread, cross platform thread library (c++)

- platforms: windows / mac
- thread create/wait/destroy
- primitives
  - mutex
  - event
  - semaphore


```c++
static void highest_priority_thread_fn(void* arg)
{
    xthreading* threading = static_cast<xthreading>(arg);
    xthread* current = threading->current_thread();

    // Here the high priority thread code
    bool active = true;
    while (active)
    {

        active = false;
    }
}

xthreading* threading = xthreading::create(xalloc::get_system());

void* my_arg = threading;
xthread* thread_main = threading->create_thread("main thread", my_arg, highest_priority_thread_fn, xthreading::HIGHEST_PRIORITY);

xmutex* mutex = threading->create_mutex("shared resource lock");
xevent* event = threading->create_event("resource locked");

threading->wait(thread_main);
threading->destroy(thread_main);
threading->destroy(mutex);
threading->destroy(event);
threading->destroy();
```