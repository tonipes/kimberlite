#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "memory.h"

typedef uint32_t Hash;

typedef struct {
  Hash hash;
  uint32_t tail;
  uint32_t count;
  uint32_t size;
} HashGen;

void kb_hash_begin  (HashGen* gen);
void kb_hash_add    (HashGen* gen, const void* data, int len);
Hash kb_hash_end    (HashGen* gen);

Hash kb_hash_memory (const Memory mem);
Hash kb_hash_string (const char* str);

#ifdef __cplusplus
}
#endif
