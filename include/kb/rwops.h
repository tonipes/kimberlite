#pragma once

#include <kb/api.h>
#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KB_RWOPS_SEEK_BEG = 0,
  KB_RWOPS_SEEK_CUR = 1,
  KB_RWOPS_SEEK_END = 2
} FileWhence;

typedef enum {
  KB_READ   = 0,
  KB_WRITE  = 1
} FileMode;

typedef struct RWops {
  int64_t   (*size)   (struct RWops* rwops);
  int64_t   (*seek)   (struct RWops* rwops, int64_t offset, int whence);
  uint64_t  (*read)   (struct RWops* rwops, void *ptr, uint64_t size);
  int       (*close)  (struct RWops* rwops);
  int64_t   (*tell)   (struct RWops* rwops);
  uint64_t  (*write)  (struct RWops* rwops, const void *ptr, uint64_t size);

  void* impl;
} RWops;

KB_API RWops*  kb_rwops_open_file  (const char* path, FileMode mode);
KB_API RWops*  kb_rwops_open_mem   (const void* dst, uint64_t size);

KB_API_INLINE uint64_t kb_rwops_seek(RWops* rwops, int64_t offset, FileWhence whence) {
  if (rwops == NULL) return 0;
  return rwops->seek(rwops, offset, whence);
}

KB_API_INLINE uint64_t kb_rwops_read(RWops* rwops, void* dst, uint64_t size) {
  if (rwops == NULL) return 0;
  return rwops->read(rwops, dst, size);
}

KB_API_INLINE uint64_t kb_rwops_write(RWops* rwops, const void* src, uint64_t size) {
  if (rwops == NULL) return 0;
  return rwops->write(rwops, src, size);
}

KB_API_INLINE int kb_rwops_close(RWops* rwops) {
  if (rwops == NULL) return 0;
  return rwops->close(rwops);
}

KB_API_INLINE int64_t kb_rwops_tell(RWops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->tell(rwops);
}

KB_API_INLINE int64_t kb_rwops_size(RWops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->size(rwops);
}

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus
template <typename T> 
KB_API_INLINE uint64_t kb_write(RWops* rwops, const T& t) {
  return kb_rwops_write(rwops, &t, sizeof(T));
}

template <typename T> 
KB_API_INLINE uint64_t kb_read(RWops* rwops, T& t) {
  return kb_rwops_read(rwops, &t, sizeof(T));
}
#endif
