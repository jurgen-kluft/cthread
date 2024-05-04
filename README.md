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
    void start(thread_t* t, thread_data_t* d)
    {
        m_thread = t;
        m_data = d;
    }
  
    void run()
    {
        bool active = true;
        while (active)
        {
            active = false;
        }
    }
  
    void exit()
    {
      
    }

protected:
    thread_t*      m_thread;
    thread_data_t* m_data;
};

threading_t* threading = threading_t::create(alloc_t::get_system());

thread_t* thread = threading->create_thread("main thread", highest_priority_thread_fn, threading_t::HIGHEST_PRIORITY);
mutex_t* mutex = threading->create_mutex("shared resource lock");
event_t* event = threading->create_event("resource locked");

threading->wait(thread);

threading->destroy(thread);
threading->destroy(mutex);
threading->destroy(event);
threading->destroy();

threading::destroy(threading);
```
