// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"
#include "alloc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_whence {
  KB_RWOPS_SEEK_BEG       = 0,
  KB_RWOPS_SEEK_CUR       = 1,
  KB_RWOPS_SEEK_END       = 2
} kb_whence;

typedef enum kb_file_mode {
  KB_FILE_MODE_UNKNOWN    = 0,
  KB_FILE_MODE_READ       = 1,
  KB_FILE_MODE_WRITE      = 2
} kb_file_mode;

typedef struct kb_stream {
  int64_t   (*size)   (struct kb_stream* rwops);
  int64_t   (*seek)   (struct kb_stream* rwops, int64_t offset, kb_whence whence);
  int64_t   (*read)   (struct kb_stream* rwops, void *ptr, int64_t size, int64_t count);
  int64_t   (*write)  (struct kb_stream* rwops, const void *ptr, int64_t size, int64_t count);
  int       (*close)  (struct kb_stream* rwops);
  int64_t   (*tell)   (struct kb_stream* rwops);
  void*     impl;
  
  char*     mem_ptr;  // Used with memory rwops
  uint64_t  mem_pos;  // Used with memory rwops
  uint64_t  mem_size; // Used with memory rwops
} kb_stream;

KB_API_INLINE int64_t kb_stream_seek(kb_stream* rwops, int64_t offset, kb_whence whence) {
  if (rwops == NULL) return -1;
  return rwops->seek(rwops, offset, whence);
}

KB_API_INLINE int64_t kb_stream_read(kb_stream* rwops, void* dst, int64_t size, int64_t count) {
  if (rwops == NULL) return 0;
  return rwops->read(rwops, dst, size, count);
}

KB_API_INLINE int64_t kb_stream_write(kb_stream* rwops, const void* src, int64_t size, int64_t count) {
  if (rwops == NULL) return 0;
  return rwops->write(rwops, src, size, count);
}

KB_API_INLINE int kb_stream_close(kb_stream* rwops) {
  if (rwops == NULL) return -1;
  int res = rwops->close(rwops);
  KB_DEFAULT_FREE(rwops);
  return res;
}

KB_API_INLINE int64_t kb_stream_tell(kb_stream* rwops) {
  if (rwops == NULL) return -1;
  return rwops->tell(rwops);
}

KB_API_INLINE int64_t kb_stream_size(kb_stream* rwops) {
  if (rwops == NULL) return -1;
  return rwops->size(rwops);
}

KB_API_INLINE bool kb_stream_check_magic(kb_stream* rwops, uint32_t magic) {
  uint32_t chunk;
  int64_t c = kb_stream_read(rwops, &chunk, sizeof(uint32_t), 1);
  return chunk == magic && c == sizeof(chunk);
}

KB_API kb_stream*  kb_stream_open_mem(void* ptr, int64_t size);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template <typename T> 
KB_API_INLINE uint64_t kb_write(kb_stream* rwops, const T& t) {
  return kb_stream_write(rwops, &t, sizeof(T), 1);
}

template <typename T> 
KB_API_INLINE uint64_t kb_read(kb_stream* rwops, T& t) {
  return kb_stream_read(rwops, &t, sizeof(T), 1);
}
#endif
