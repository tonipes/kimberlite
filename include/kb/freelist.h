#pragma once

#include <kb/api.h>
#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct {
  uint16_t*   handles;
  uint16_t    count;
  uint16_t    capacity;
} Freelist;

KB_API void       kb_freelist_create        (Freelist* freelist, uint16_t capacity);
KB_API void       kb_freelist_destroy       (Freelist* freelist);
KB_API void       kb_freelist_reset         (Freelist* freelist);
KB_API uint16_t   kb_freelist_take          (Freelist* freelist);
KB_API void       kb_freelist_return        (Freelist* freelist, uint16_t handle);
KB_API uint16_t   kb_freelist_count         (const Freelist* freelist);
KB_API uint16_t*  kb_freelist_get_sparse    (Freelist* freelist);
KB_API uint16_t*  kb_freelist_get_dense     (Freelist* freelist);

#ifdef __cplusplus
}
#endif
