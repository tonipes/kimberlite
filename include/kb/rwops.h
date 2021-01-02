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

typedef struct kb_rwops {
  int64_t   (*size)   (struct kb_rwops* rwops);
  int64_t   (*seek)   (struct kb_rwops* rwops, int64_t offset, int whence);
  uint64_t  (*read)   (struct kb_rwops* rwops, void *ptr, uint64_t size);
  int       (*close)  (struct kb_rwops* rwops);
  int64_t   (*tell)   (struct kb_rwops* rwops);
  uint64_t  (*write)  (struct kb_rwops* rwops, const void *ptr, uint64_t size);
  void*     impl; 
  uint64_t  mem_pos;  // Used with memory rwops
  uint64_t  mem_size; // Used with memory rwops

} kb_rwops;

KB_API kb_rwops*  kb_rwops_open_file  (const char* path, kb_file_mode mode);
KB_API kb_rwops*  kb_rwops_open_mem   (const void* ptr, uint64_t size);

KB_API_INLINE int64_t kb_rwops_seek(kb_rwops* rwops, int64_t offset, kb_whence whence) {
  if (rwops == NULL) return -1;
  return rwops->seek(rwops, offset, whence);
}

KB_API_INLINE uint64_t kb_rwops_read(kb_rwops* rwops, void* dst, uint64_t size) {
  if (rwops == NULL) return 0;
  return rwops->read(rwops, dst, size);
}

KB_API_INLINE uint64_t kb_rwops_write(kb_rwops* rwops, const void* src, uint64_t size) {
  if (rwops == NULL) return 0;
  return rwops->write(rwops, src, size);
}

KB_API_INLINE int kb_rwops_close(kb_rwops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->close(rwops);
}

KB_API_INLINE int64_t kb_rwops_tell(kb_rwops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->tell(rwops);
}

KB_API_INLINE int64_t kb_rwops_size(kb_rwops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->size(rwops);
}

KB_API_INLINE bool kb_rwops_check_magic(kb_rwops* rwops, uint32_t magic) {
  uint32_t chunk;
  int32_t c = kb_rwops_read(rwops, &chunk, sizeof(uint32_t));
  return chunk == magic && c == sizeof(chunk);
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
template <typename T> 
KB_API_INLINE uint64_t kb_write(kb_rwops* rwops, const T& t) {
  return kb_rwops_write(rwops, &t, sizeof(T));
}

template <typename T> 
KB_API_INLINE uint64_t kb_read(kb_rwops* rwops, T& t) {
  return kb_rwops_read(rwops, &t, sizeof(T));
}
#endif
