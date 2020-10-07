#pragma once

#include <kb/core.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t kb_hash;

typedef struct {
  kb_hash   hash;
  uint32_t  tail;
  uint32_t  count;
  uint32_t  size;
} kb_hash_gen;

KB_API void     kb_hash_begin  (kb_hash_gen* gen);
KB_API void     kb_hash_add    (kb_hash_gen* gen, const void* data, int len);
KB_API kb_hash  kb_hash_end    (kb_hash_gen* gen);
KB_API kb_hash  kb_hash_string (const char* str);

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

template <typename T> 
KB_API_INLINE void kb_hash_add(kb_hash_gen* gen, const T& t) {
  kb_hash_add(gen, &t, sizeof(T));
}

#endif
