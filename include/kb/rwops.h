#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define RWOPS_SEEK_SET 0
#define RWOPS_SEEK_CUR 1
#define RWOPS_SEEK_END 2

typedef struct RWops {
  int64_t   (*size)   (struct RWops* rwops);
  int64_t   (*seek)   (struct RWops* rwops, int64_t offset, int whence);
  uint64_t  (*read)   (struct RWops* rwops, void *ptr, uint64_t count);
  int       (*close)  (struct RWops* rwops);
  int64_t   (*tell)   (struct RWops* rwops);
  uint64_t  (*write)  (struct RWops* rwops, const void *ptr, uint64_t count);

  void* impl;
} RWops;

RWops*  kb_rwops_open_file  (const char* path);
RWops*  kb_rwops_open_mem   (const void* dst, uint64_t size);

inline uint64_t kb_rwops_seek(RWops* rwops, int64_t offset, int whence) {
  return rwops->seek(rwops, offset, whence);
}

inline uint64_t kb_rwops_read(RWops* rwops, void* dst, uint64_t count) {
  return rwops->read(rwops, dst, count);
}

inline uint64_t kb_rwops_write(RWops* rwops, const void* src, uint64_t count) {
  return rwops->write(rwops, src, count);
}

inline int kb_rwops_close(RWops* rwops) {
  return rwops->close(rwops);
}

inline int64_t kb_rwops_tell(RWops* rwops) {
  return rwops->tell(rwops);
}

inline int64_t kb_rwops_size(RWops* rwops) {
  return rwops->size(rwops);
}

#ifdef __cplusplus
}
#endif
