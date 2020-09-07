#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define RWOPS_SEEK_SET 0
#define RWOPS_SEEK_CUR 1
#define RWOPS_SEEK_END 2

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

RWops*  kb_rwops_open_file  (const char* path, FileMode mode);
RWops*  kb_rwops_open_mem   (const void* dst, uint64_t size);

inline uint64_t kb_rwops_seek(RWops* rwops, int64_t offset, int whence) {
  if (rwops == NULL) return 0;
  return rwops->seek(rwops, offset, whence);
}

inline uint64_t kb_rwops_read(RWops* rwops, void* dst, uint64_t size) {
  if (rwops == NULL) return 0;
  return rwops->read(rwops, dst, size);
}

inline uint64_t kb_rwops_write(RWops* rwops, const void* src, uint64_t size) {
  if (rwops == NULL) return 0;
  return rwops->write(rwops, src, size);
}

inline int kb_rwops_close(RWops* rwops) {
  if (rwops == NULL) return 0;
  return rwops->close(rwops);
}

inline int64_t kb_rwops_tell(RWops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->tell(rwops);
}

inline int64_t kb_rwops_size(RWops* rwops) {
  if (rwops == NULL) return -1;
  return rwops->size(rwops);
}

#ifdef __cplusplus
}
#endif
