#pragma once

#include <kb/core.h>
#include <kb/hash.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_vertex_attribute_type {
  KB_ATTRIB_FLOAT = 0,
  KB_ATTRIB_SINT  = 1,
  KB_ATTRIB_UINT  = 2,
} kb_vertex_attribute_type;

typedef struct kb_vertex_attribute {
  uint32_t                  location;
  uint32_t                  count;
  uint32_t                  offset;
  uint32_t                  size;
  kb_vertex_attribute_type  type;
} kb_vertex_attribute;

typedef struct kb_vertex_layout {
  uint32_t                  attrib_count;
  kb_vertex_attribute       attribs[KB_CONFIG_MAX_VERTEX_ATTRIBS];
  kb_hash                   hash;
} kb_vertex_layout;

KB_API void      kb_vertex_layout_begin         (kb_vertex_layout* layout);
KB_API uint32_t  kb_vertex_layout_add           (kb_vertex_layout* layout, kb_vertex_attribute_type type, uint32_t num);
KB_API void      kb_vertex_layout_end           (kb_vertex_layout* layout);
KB_API void      kb_vertex_layout_load          (kb_vertex_layout* layout, void* spirv_code, uint32_t code_size);
KB_API uint32_t  kb_vertex_layout_stride        (const kb_vertex_layout* layout);
KB_API uint32_t  kb_vertex_layout_offset        (const kb_vertex_layout* layout, uint32_t attrib);
KB_API uint32_t  kb_vertex_layout_size          (const kb_vertex_layout* layout, uint32_t attrib);
KB_API void      kb_vertex_layout_dump          (const kb_vertex_layout* layout);
KB_API bool      kb_vertex_layout_match         (const kb_vertex_layout* a, const kb_vertex_layout* b);
KB_API uint32_t  kb_vertex_layout_attrib_size   (kb_vertex_attribute_type type, uint32_t num);

#ifdef __cplusplus
}
#endif
