#pragma once

#include <kb/api.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  struct MutexImpl* impl;
} Mutex;

typedef struct {
  struct SemaphoreImpl* impl;
} Semaphore;

typedef struct {
  struct ThreadPoolInternal* internal;
} ThreadPool;

typedef void(*JobFunc)(void*);

KB_API ThreadPool  kb_threadpool_create            (int num_threads);
KB_API void        kb_threadpool_destroy           (ThreadPool pool);
KB_API void        kb_threadpool_wait              (ThreadPool pool);
KB_API void        kb_threadpool_pause             (ThreadPool pool);
KB_API void        kb_threadpool_resume            (ThreadPool pool);
KB_API int         kb_threadpool_active_threads    (ThreadPool pool);
KB_API int         kb_threadpool_queue_length      (ThreadPool pool);
KB_API int         kb_threadpool_add_job           (ThreadPool pool, void* param, JobFunc job);

KB_API Semaphore   kb_semaphore_create             (bool value);
KB_API void        kb_semaphore_destroy            (Semaphore semaphore);
KB_API void        kb_semaphore_init               (Semaphore semaphore, bool value);
KB_API void        kb_semaphore_reset              (Semaphore semaphore);
KB_API void        kb_semaphore_post               (Semaphore semaphore);
KB_API void        kb_semaphore_post_all           (Semaphore semaphore);
KB_API void        kb_semaphore_wait               (Semaphore semaphore);

KB_API Mutex       kb_mutex_create                 ();
KB_API void        kb_mutex_destroy                (Mutex mutex);
KB_API void        kb_mutex_lock                   (Mutex mutex);
KB_API void        kb_mutex_unlock                 (Mutex mutex);

#ifdef __cplusplus
}

#endif
