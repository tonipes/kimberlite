#pragma once

#include <kb/api.h>
#include <kb/types.h>
#include <kb/memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t Hash;

typedef struct {
  Hash hash;
  uint32_t tail;
  uint32_t count;
  uint32_t size;
} HashGen;

KB_API void kb_hash_begin  (HashGen* gen);
KB_API void kb_hash_add    (HashGen* gen, const void* data, int len);
KB_API Hash kb_hash_end    (HashGen* gen);

KB_API Hash kb_hash_memory (const Memory mem);
KB_API Hash kb_hash_string (const char* str);

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

template <typename T> 
KB_API_INLINE void kb_hash_add(HashGen* gen, const T& t) {
  kb_hash_add(gen, &t, sizeof(T));
}

#endif
