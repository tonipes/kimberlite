#pragma once

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

ThreadPool  kb_threadpool_create            (int num_threads);
void        kb_threadpool_destroy           (ThreadPool pool);
void        kb_threadpool_wait              (ThreadPool pool);
void        kb_threadpool_pause             (ThreadPool pool);
void        kb_threadpool_resume            (ThreadPool pool);
int         kb_threadpool_active_threads    (ThreadPool pool);
int         kb_threadpool_queue_length      (ThreadPool pool);
int         kb_threadpool_add_job           (ThreadPool pool, void* param, JobFunc job);

Semaphore   kb_semaphore_create             (bool value);
void        kb_semaphore_destroy            (Semaphore semaphore);
void        kb_semaphore_init               (Semaphore semaphore, bool value);
void        kb_semaphore_reset              (Semaphore semaphore);
void        kb_semaphore_post               (Semaphore semaphore);
void        kb_semaphore_post_all           (Semaphore semaphore);
void        kb_semaphore_wait               (Semaphore semaphore);

Mutex       kb_mutex_create                 ();
void        kb_mutex_destroy                (Mutex mutex);
void        kb_mutex_lock                   (Mutex mutex);
void        kb_mutex_unlock                 (Mutex mutex);

#ifdef __cplusplus
}

#endif
