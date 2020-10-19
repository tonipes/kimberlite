// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_mutex       kb_mutex;
typedef struct kb_semaphore   kb_semaphore;
typedef struct kb_thread_pool kb_thread_pool;

typedef void(*kb_job_func)(void*);

KB_API kb_thread_pool*  kb_threadpool_create              (int num_threads);
KB_API void             kb_threadpool_destroy             (kb_thread_pool* pool);
KB_API void             kb_threadpool_wait                (kb_thread_pool* pool);
KB_API void             kb_threadpool_pause               (kb_thread_pool* pool);
KB_API void             kb_threadpool_resume              (kb_thread_pool* pool);
KB_API int              kb_threadpool_active_threads      (kb_thread_pool* pool);
KB_API int              kb_threadpool_queue_length        (kb_thread_pool* pool);
KB_API int              kb_threadpool_add_job             (kb_thread_pool* pool, void* param, kb_job_func job);

KB_API kb_semaphore*    kb_semaphore_create               (bool value);
KB_API void             kb_semaphore_destroy              (kb_semaphore* semaphore);
KB_API void             kb_semaphore_init                 (kb_semaphore* semaphore, bool value);
KB_API void             kb_semaphore_reset                (kb_semaphore* semaphore);
KB_API void             kb_semaphore_post                 (kb_semaphore* semaphore);
KB_API void             kb_semaphore_post_all             (kb_semaphore* semaphore);
KB_API void             kb_semaphore_wait                 (kb_semaphore* semaphore);

KB_API kb_mutex*        kb_mutex_create                   ();
KB_API void             kb_mutex_destroy                  (kb_mutex* mutex);
KB_API void             kb_mutex_lock                     (kb_mutex* mutex);
KB_API void             kb_mutex_unlock                   (kb_mutex* mutex);

#ifdef __cplusplus
}

#endif
