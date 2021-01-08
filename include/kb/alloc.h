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

#define KB_DEFAULT_ALIGN 8

typedef struct kb_alloc_stats {
  uint64_t        count;
  uint64_t        mem;
  uint64_t        high_water_mark;
} kb_alloc_stats;

typedef struct kb_allocator {
  void* (*realloc) (struct kb_allocator*, void*, size_t, size_t);
  kb_alloc_stats  stats;
  void*           impl;
} kb_allocator;

KB_API void* kb_alloc    (kb_allocator* alloc, size_t size, size_t align, const char* file, const char* line);
KB_API void* kb_realloc  (kb_allocator* alloc, void* ptr, size_t size, size_t align, const char* file, const char* line);
KB_API void  kb_free     (kb_allocator* alloc, void* ptr, const char* file, const char* line);

KB_API uint32_t kb_alloc_count              (kb_allocator* alloc);
KB_API uint64_t kb_alloc_mem                (kb_allocator* alloc);
KB_API uint64_t kb_alloc_high_water_mark    (kb_allocator* alloc);

#define KB_ALLOC(alloc, size)                               kb_alloc    (alloc, size,                      KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_ALLOC_ALIGN(alloc, size, align)                  kb_alloc    (alloc, size,                      align, NULL, NULL)
#define KB_ALLOC_TYPE(alloc, type, count)           (type*) kb_alloc    (alloc, sizeof(type) * count,      KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_REALLOC_TYPE(alloc, type, ptr, count)    (type*) kb_realloc  (alloc, ptr, sizeof(type) * count, KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_REALLOC(alloc, ptr, size)                        kb_realloc  (alloc, ptr, size,                 KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_FREE(alloc, ptr)                                 kb_free     (alloc, ptr, NULL, NULL)

#define KB_DEFAULT_ALLOC(size)                              kb_alloc    (NULL, size,                       KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_DEFAULT_ALLOC_ALIGN(size, align)                 kb_alloc    (NULL, size,                       align, NULL, NULL)
#define KB_DEFAULT_ALLOC_TYPE(type, count)          (type*) kb_alloc    (NULL, sizeof(type) * count,       KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_DEFAULT_REALLOC_TYPE(type, ptr, count)   (type*) kb_realloc  (NULL, ptr, sizeof(type) * count,  KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_DEFAULT_REALLOC(ptr, size)                       kb_realloc  (NULL, ptr, size,                  KB_DEFAULT_ALIGN, NULL, NULL)
#define KB_DEFAULT_FREE(ptr)                                kb_free     (NULL, ptr, NULL, NULL)

#ifdef __cplusplus
}
#endif
