#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define KB_DEFAULT_ALIGN 8

typedef void*(*ReallocFunc)(void*, size_t, size_t);

typedef struct {
  ReallocFunc realloc;
} Allocator;

extern Allocator* default_alloc;

void* kb_alloc    (Allocator* alloc, size_t size, size_t align);
void* kb_realloc  (Allocator* alloc, void* ptr, size_t size, size_t align);
void  kb_free     (Allocator* alloc, void* ptr, size_t align);

#define KB_ALLOC(alloc, size)         kb_alloc    (alloc, size,       KB_DEFAULT_ALIGN)
#define KB_REALLOC(alloc, ptr, size)  kb_realloc  (alloc, ptr, size,  KB_DEFAULT_ALIGN)
#define KB_FREE(alloc, ptr)           kb_free     (alloc, ptr,        KB_DEFAULT_ALIGN)

#ifdef __cplusplus
}
#endif
