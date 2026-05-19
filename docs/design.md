# cthread Design

## Purpose

cthread provides a small cross-platform threading and synchronization layer for the ncore codebase. It wraps platform primitives (pthread/mach on macOS, Win32 on Windows, mock implementations for tests) behind a consistent API and central allocator-managed object pools.

The library is designed around:

- Explicit system initialization and teardown.
- Fixed-capacity pools for deterministic allocation.
- Opaque platform-specific data structs behind public handles.
- Basic primitives: threads, mutexes, events, semaphores.

## Scope

In scope:

- Thread creation and joining.
- Thread priority abstraction.
- Mutex lock/unlock and try-lock.
- Event wait/signal/reset.
- Semaphore wait/signal/timed wait.
- Centralized allocator ownership via system_t.

Out of scope:

- Work stealing or thread pools.
- Condition variables.
- Lock-free containers.
- Affinity and advanced scheduler policy configuration.

## Public API Shape

The current public headers expose a function-oriented API in namespace ncore::nthread.

Primary entry points:

- create_system(alloc_t* allocator, u32 max_threads, u32 max_mutex, u32 max_event, u32 max_semaphore)
- destroy_system(system_t*&)
- create_thread(...)
- create_mutex()
- create_event(const char* name, bool autoReset)
- create_sema(s32 initial_count, s32 max_count)
- destroy(thread_t*), destroy(mutex_t*), destroy(event_t*), destroy(sema_t*)
- join(thread_t*)
- sleep(u32), yield(), quit(), current()

Thread callbacks are plain C-style function pointers:

- start_fn
- run_fn
- exit_fn
- quit_fn

State and priority are compact enums:

- nstate: CREATED, RUNNING, STOPPED
- npriority: IDLE to CRITICAL

## Core Architecture

### 1) Global System Instance

The runtime is anchored by a global pointer:

- s_system: system_t*

system_t owns allocator and all fixed pools. create_system sets up pools and initializes the platform priority map. destroy_system tears down pools and deallocates system memory.

### 2) Fixed Pools

system_t contains typed fixed pools for both user-facing objects and platform data:

- thread_t + thread_data_t
- mutex_t + mutex_data_t
- event_t + event_data_t
- sema_t + sema_data_t

This gives bounded capacity and predictable allocation cost.

### 3) Opaque Platform Data

Public handles hold pointers to internal data blocks:

- thread_t::m_data
- mutex_t::m_data
- event_t::m_data
- sema_t::m_data

Internal data layout differs per platform and is defined in:

- source/main/include/cthread/private/c_thread_mac.h
- source/main/include/cthread/private/c_thread_win.h
- source/main/include/cthread/private/c_thread_mock.h

## Thread Lifecycle Model

The intended lifecycle is:

1. Allocate thread_t/thread_data_t from pools.
2. Fill metadata (name, stack size, priority, state=CREATED).
3. Start native thread.
4. Inside native entry:
	- assign native thread id
	- run start_fn
	- set state RUNNING
	- run run_fn
	- set state STOPPED
	- run exit_fn
5. Join from another thread if required.
6. Return object/data to pools.

On macOS, native thread entry uses a thread-local pointer to the current thread_data_t.

## Primitive Semantics

### Mutex

- Recursive behavior is documented in the header comment.
- API: 
  - `lock`
  - `tryLock`
  - `unlock`
- macOS uses `pthread_mutex`.
- Windows legacy implementation uses `CRITICAL_SECTION`.

### Event

- API: 
  - `set`
  - `wait`
  - `reset`
- Conceptually supports auto-reset/manual-reset behavior.
- macOS implementation uses an atomic status counter plus a lightweight semaphore.

### Semaphore

- API: 
  - `signal`
  - `wait`
  - `try_wait(milliseconds)`
- macOS uses Mach semaphores.
- Windows legacy implementation uses Win32 semaphore handles.

## Platform Backends

### macOS

macOS backend is the most complete in the current tree for the function-oriented thread API:

- `source/main/cpp/mac/c_thread_mac.cpp`
- `source/main/cpp/mac/c_mutex_mac.cpp`
- `source/main/cpp/mac/c_event_mac.cpp`
- `source/main/cpp/mac/c_semaphore_mac.cpp`

Implemented features include sleep/yield/quit and priority mapping initialization.

### Windows

Windows backend files currently follow an older class-based API style (threading_t/thread_fn_t) and include TODO stubs in threading support functions.

### Mock (TARGET_TEST)

Mock backend also follows the older class-based API style and is mostly behavior stubs.

## Memory Ownership and Allocation

- All runtime objects come from system_t fixed pools.
- system_t itself is allocated through the caller-provided alloc_t.
- The API is intended to require explicit destroy(...) for each allocated primitive plus destroy_system for the runtime.

## Error Handling Strategy

- C-style return codes are used for thread construct/join functions.
- Allocation failures return nullptr.
- Platform calls generally do best-effort execution with minimal propagation of detailed OS errors.

## Observed Implementation Status and Gaps

The repository currently contains a transition between an older class-based API and a newer function-oriented API. The following points are important for maintainers:

1. API split across source files.
2. create_thread is declared in the function API header but no matching implementation is present in source/main/cpp/c_threading.cpp.
3. create_mutex allocates mutex_data_t but does not attach it to mutex_t::m_data in source/main/cpp/c_threading.cpp.
4. destroy(...) functions return pool memory but do not invoke *_data_release to release platform handles/objects.
5. system_t::setup and system_t::teardown call thread pool setup/teardown twice.
6. macOS event_data_init receives autoReset but does not use it.
7. macOS thread entry auto-destroys thread objects on thread exit; this can conflict with explicit destroy(thread_t*) usage.
8. Header/source naming mismatch exists for thread_get_data/thread_get_system.
9. source/main/include/cthread/c_scopedlock.h contains constructor parameter issues that need correction.
10. Tests in source/test/cpp are written against the older threading_t API and do not match current public headers.

## Recommended Direction

To stabilize cthread, pick one API model and complete it end-to-end.

Preferred path:

1. Keep the function-oriented nthread API from current headers.
2. Port all backend sources to that API shape.
3. Make destruction rules explicit (manual destroy vs auto-destroy) and enforce one policy.
4. Ensure *_data_init and *_data_release are always paired.
5. Update tests to the chosen API and run on all supported targets.

This will make behavior predictable and reduce platform divergence.
