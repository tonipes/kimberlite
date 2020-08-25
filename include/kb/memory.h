#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef struct {
  const void* data;
  uint32_t    size;
} Memory;

Memory get_str_memory(const char*);

#define KB_STATIC_MEMORY(src) { (void*)src, sizeof(src) }

#ifdef __cplusplus
}
#endif
