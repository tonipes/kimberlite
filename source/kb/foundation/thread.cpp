// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/thread.h>

#include <kb/foundation/alloc.h>
#include <kb/foundation/crt.h>

#include <pthread.h>
#include <signal.h>
#include <unistd.h>

const static double KILL_TIMEOUT = 1.0;

typedef struct kb_mutex {
  pthread_mutex_t id;
} kb_mutex;

typedef struct kb_semaphore {
  kb_mutex*         mutex;
  pthread_cond_t    cond;
  bool              value;
} kb_semaphore;

typedef struct kb_job {
  kb_job*           prev;
  kb_job_func       func;
  void*             userdata;
} kb_job;

typedef struct kb_thread {
  pthread_t         impl;
  void*             userdata;
} kb_thread;

typedef struct kb_job_queue {
  kb_mutex*         mutex;
  kb_job*           front;
  kb_job*           rear;
  kb_semaphore*     has_jobs;
  int               len;
} kb_job_queue;

typedef struct kb_thread_pool {
  kb_thread**       threads;
  volatile int      num_threads_alive;
  volatile int      num_threads_working;
  kb_mutex*         thread_count_mutex;
  pthread_cond_t    threads_all_idle;
  kb_job_queue      jobqueue;
  bool              value;
  volatile bool     pool_alive;
} kb_thread_pool;

void*     threadpool_do         (void* data);
void      thread_sig_handler    (int, siginfo_t*, void*);

int       jobqueue_init         (kb_job_queue* queue);
void      jobqueue_clear        (kb_job_queue* queue);
void      jobqueue_push         (kb_job_queue* queue, kb_job* job);
kb_job*   jobqueue_pull         (kb_job_queue* queue);
void      jobqueue_destroy      (kb_job_queue* queue);

KB_API kb_thread* kb_thread_create(kb_thread_func func, void* userdata) {
  kb_thread* thread;
  thread = (kb_thread*) KB_DEFAULT_ALLOC(sizeof(kb_thread));

  thread->userdata = userdata;
  pthread_create(&thread->impl, NULL, func, thread->userdata);
  
  return thread;
}

KB_API void kb_thread_join(kb_thread* thread) {
  pthread_join(thread->impl, NULL);
}

KB_API void kb_thread_destroy(kb_thread* thread) {
  KB_DEFAULT_FREE(thread);
}

kb_thread_pool* kb_threadpool_create(int num_threads) {
  if (num_threads < 0){
    num_threads = 0;
  }
  
  kb_thread_pool* pool;
  pool = (kb_thread_pool*) KB_DEFAULT_ALLOC(sizeof(kb_thread_pool));
  
  if (pool == NULL) {
    return pool;
  }
  
  pool->num_threads_alive    = 0;
  pool->num_threads_working  = 0;
  pool->pool_alive           = 1;

  if (jobqueue_init(&pool->jobqueue) == -1){
    KB_DEFAULT_FREE(pool);
    return pool;
  }
  
  pool->thread_count_mutex = kb_mutex_create();
  
  pthread_cond_init(&pool->threads_all_idle, NULL);

  pool->threads     = KB_DEFAULT_ALLOC_TYPE(kb_thread*, num_threads);
  
  for (int n = 0; n < num_threads; n++) {
    pool->threads[n] = kb_thread_create(threadpool_do, pool);
  }
  
  // Wait for threads to signal
  while (pool->num_threads_alive != num_threads) { }

  return pool;
}
 
void kb_threadpool_wait(kb_thread_pool* pool) {
  kb_mutex_lock(pool->thread_count_mutex);

  while (pool->jobqueue.len || pool->num_threads_working) {
    pthread_cond_wait(&pool->threads_all_idle, &pool->thread_count_mutex->id);
  }

  kb_mutex_unlock(pool->thread_count_mutex);
}

void kb_threadpool_pause(kb_thread_pool* pool) {
  KB_NOT_IMPLEMENTED("kb_threadpool_pause");
}

void kb_threadpool_resume(kb_thread_pool* pool) {
  KB_NOT_IMPLEMENTED("kb_threadpool_resume");
}

int kb_threadpool_queue_length(kb_thread_pool* pool) {
  kb_mutex_lock(pool->jobqueue.mutex);
  int len = pool->jobqueue.len;
  kb_mutex_unlock(pool->jobqueue.mutex);

  return len;
}

int kb_threadpool_add_job(kb_thread_pool* pool, void* userdata, kb_job_func func) {
  kb_job* job;

  job = (kb_job*) KB_DEFAULT_ALLOC(sizeof(kb_job));

  if (job == NULL){
    return -1;
  }

  job->func=func;
  job->userdata = userdata;

  jobqueue_push(&pool->jobqueue, job);
  return 0;
}

void kb_threadpool_destroy(kb_thread_pool* pool) {
  if (pool == NULL) return;

  volatile int thread_count = pool->num_threads_alive;

  pool->pool_alive = 0;

  time_t start, end;
  double tpassed = 0.0;
  time (&start);

  while (tpassed < KILL_TIMEOUT && pool->num_threads_alive) {
    kb_semaphore_post_all(pool->jobqueue.has_jobs);
    time (&end);
    tpassed = difftime(end,start);
  }

  while (pool->num_threads_alive) {
    kb_semaphore_post_all(pool->jobqueue.has_jobs);
    sleep(1);
  }

  jobqueue_destroy(&pool->jobqueue);

  for (int n = 0; n < thread_count; n++) {
    kb_thread_destroy(pool->threads[n]);
    // thread_destroy(&pool->threads[n]);
  }

  KB_DEFAULT_FREE(pool->threads);
  KB_DEFAULT_FREE(pool);
}

int kb_threadpool_active_threads(kb_thread_pool* pool) {
  return pool->num_threads_working;
}

void* threadpool_do(void* userdata) {
  kb_thread_pool* impl = (kb_thread_pool*) userdata;
    
  struct sigaction act;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_SIGINFO;
  act.sa_sigaction = thread_sig_handler;

  kb_mutex_lock(impl->thread_count_mutex);
  impl->num_threads_alive++;
  kb_mutex_unlock(impl->thread_count_mutex);
  
  while (impl->pool_alive) {
    kb_semaphore_wait(impl->jobqueue.has_jobs);
    
    if (impl->pool_alive) {
      
      kb_mutex_lock(impl->thread_count_mutex);

      impl->num_threads_working++;
      kb_mutex_unlock(impl->thread_count_mutex);

      // Take a job
      kb_job* job = jobqueue_pull(&impl->jobqueue);			

      if (job) {
        kb_job_func function_buffer = job->func;
        void* param_buffer = job->userdata;
        
        function_buffer(param_buffer);

        KB_DEFAULT_FREE(job);
      }
      
      // Thread idle
      kb_mutex_lock(impl->thread_count_mutex);

      impl->num_threads_working--;
      if (!impl->num_threads_working) {
        pthread_cond_signal(&impl->threads_all_idle);
      }
      kb_mutex_unlock(impl->thread_count_mutex);

    }
  }
  
  // Thread dead
  kb_mutex_lock(impl->thread_count_mutex);
  impl->num_threads_alive--;
  kb_mutex_unlock(impl->thread_count_mutex);
  
  return NULL;
}

void thread_sig_handler(int sig, siginfo_t* info, void* uctx) {

}

void kb_semaphore_reset(kb_semaphore* sem, bool value) {
  sem->mutex = kb_mutex_create();
  pthread_cond_init(&(sem->cond), NULL);
  sem->value = value;
}

kb_semaphore* kb_semaphore_create(bool value) {
  kb_semaphore* semaphore;
  semaphore = (kb_semaphore*) KB_DEFAULT_ALLOC(sizeof(kb_semaphore));
  
  kb_semaphore_reset(semaphore, value);

  return semaphore;
}

void kb_semaphore_destroy(kb_semaphore* semaphore) {
  KB_DEFAULT_FREE(semaphore);
}

void kb_semaphore_post(kb_semaphore* sem) {
  kb_mutex_lock(sem->mutex);
  sem->value = 1;
  pthread_cond_signal(&sem->cond);
  kb_mutex_unlock(sem->mutex);
}

void kb_semaphore_post_all(kb_semaphore* sem) {
  kb_mutex_lock(sem->mutex);
  sem->value = 1;
  pthread_cond_broadcast(&sem->cond);
  kb_mutex_unlock(sem->mutex);
}

void kb_semaphore_wait(kb_semaphore* sem) {
  kb_mutex_lock(sem->mutex);

  while (sem->value != 1) {
    pthread_cond_wait(&sem->cond, &sem->mutex->id);
  }

  sem->value = 0;
  kb_mutex_unlock(sem->mutex);

}

int jobqueue_init(kb_job_queue* queue) {
  queue->len = 0;
  queue->front = NULL;
  queue->rear  = NULL;

  queue->mutex = kb_mutex_create();
  queue->has_jobs = kb_semaphore_create(0);
  
  if (queue->has_jobs == NULL){
    return -1;
  }
  
  return 0;
}

void jobqueue_clear(kb_job_queue* queue) {
  while(queue->len){
    KB_DEFAULT_FREE(jobqueue_pull(queue));
  }

  queue->front = NULL;
  queue->rear  = NULL;
  kb_semaphore_reset(queue->has_jobs, false);
  queue->len = 0;
}

void jobqueue_push(kb_job_queue* queue, kb_job* job) {
  kb_mutex_lock(queue->mutex);

  job->prev = NULL;
  
  if (queue->len == 0) {
    queue->front = job;
    queue->rear  = job;
  } else {
    queue->rear->prev = job;
    queue->rear = job;
  }

  queue->len++;

  kb_semaphore_post(queue->has_jobs);
  kb_mutex_unlock(queue->mutex);
}

kb_job* jobqueue_pull(kb_job_queue* queue) {
  kb_mutex_lock(queue->mutex);

  kb_job* job = queue->front;

  if (queue->len == 1) {
    queue->front = NULL;
    queue->rear  = NULL;
    queue->len = 0;
  } else if (queue->len > 0)  {
    queue->front = job->prev;
    queue->len--;
    kb_semaphore_post(queue->has_jobs);
  }

  kb_mutex_unlock(queue->mutex);

  return job;
}

void jobqueue_destroy(kb_job_queue* queue) {
  jobqueue_clear(queue);

  kb_semaphore_destroy(queue->has_jobs);
}

kb_mutex* kb_mutex_create() {
  kb_mutex* mutex;
  mutex = (kb_mutex*) KB_DEFAULT_ALLOC(sizeof(kb_mutex));
  
  pthread_mutex_init(&(mutex->id), NULL);

  return mutex;
}

void kb_mutex_destroy(kb_mutex* mutex) {
  KB_DEFAULT_FREE(mutex);
}

void kb_mutex_lock(kb_mutex* mutex) {
  pthread_mutex_lock(&mutex->id);
}

void kb_mutex_unlock(kb_mutex* mutex) {
  pthread_mutex_unlock(&mutex->id);
}
