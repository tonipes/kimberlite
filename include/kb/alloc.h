#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KB_DEFAULT_ALIGN 8

typedef struct kb_alloc_stats {
  uint64_t count;
  uint64_t mem;
  uint64_t high_water_mark;
} kb_alloc_stats;

typedef struct Allocator {
  void* (*realloc) (struct Allocator*, void*, size_t, size_t);
  kb_alloc_stats stats;
  void* impl;
} Allocator;

KB_API void* kb_alloc    (Allocator* alloc, size_t size, size_t align);
KB_API void* kb_realloc  (Allocator* alloc, void* ptr, size_t size, size_t align);
KB_API void  kb_free     (Allocator* alloc, void* ptr, size_t align);

KB_API uint32_t kb_alloc_count              (Allocator* alloc);
KB_API uint64_t kb_alloc_mem                (Allocator* alloc);
KB_API uint64_t kb_alloc_high_water_mark    (Allocator* alloc);

#define KB_ALLOC(alloc, size)                               kb_alloc    (alloc, size,                      KB_DEFAULT_ALIGN)
#define KB_ALLOC_TYPE(alloc, type, count)           (type*) kb_alloc    (alloc, sizeof(type) * count,      KB_DEFAULT_ALIGN)
#define KB_REALLOC_TYPE(alloc, type, ptr, count)    (type*) kb_realloc  (alloc, ptr, sizeof(type) * count, KB_DEFAULT_ALIGN)
#define KB_REALLOC(alloc, ptr, size)                        kb_realloc  (alloc, ptr, size,                 KB_DEFAULT_ALIGN)
#define KB_FREE(alloc, ptr)                                 kb_free     (alloc, ptr,                       KB_DEFAULT_ALIGN)

#define KB_DEFAULT_ALLOC(size)                              kb_alloc    (NULL, size,                       KB_DEFAULT_ALIGN)
#define KB_DEFAULT_ALLOC_TYPE(type, count)          (type*) kb_alloc    (NULL, sizeof(type) * count,       KB_DEFAULT_ALIGN)
#define KB_DEFAULT_REALLOC_TYPE(type, ptr, count)   (type*) kb_realloc  (NULL, ptr, sizeof(type) * count,  KB_DEFAULT_ALIGN)
#define KB_DEFAULT_REALLOC(ptr, size)                       kb_realloc  (NULL, ptr, size,                  KB_DEFAULT_ALIGN)
#define KB_DEFAULT_FREE(ptr)                                kb_free     (NULL, ptr,                        KB_DEFAULT_ALIGN)

// KB_API void kb_alloc_create_linear  (Allocator* dst, size_t capacity, Allocator* root);
// KB_API void kb_alloc_linear_reset   (Allocator* alloc);

#ifdef __cplusplus
}
#endif
