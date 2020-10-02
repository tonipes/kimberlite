#pragma once

#include <kb/api.h>
#include <kb/types.h>
#include <kb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KB_ATTRIB_FLOAT = 0,
  KB_ATTRIB_SINT  = 1,
  KB_ATTRIB_UINT  = 2,
} AttributeType;

typedef struct {
  uint32_t      location;
  uint32_t      count;
  uint32_t      offset;
  uint32_t      size;
  AttributeType type;
} VertexAttribute;

typedef struct {
  uint32_t attrib_count;
  VertexAttribute attribs[KB_CONFIG_MAX_VERTEX_ATTRIBS];
} VertexLayout;

KB_API uint32_t  kb_vertex_layout_attrib_size   (AttributeType type, uint32_t num);

KB_API void      kb_vertex_layout_begin         (VertexLayout* layout);
KB_API uint32_t  kb_vertex_layout_add           (VertexLayout* layout, AttributeType type, uint32_t num);
KB_API void      kb_vertex_layout_end           (VertexLayout* layout);

KB_API void      kb_vertex_layout_load          (VertexLayout* layout, void* spirv_code, uint32_t code_size);

KB_API uint32_t  kb_vertex_layout_stride        (const VertexLayout* layout);
KB_API uint32_t  kb_vertex_layout_offset        (const VertexLayout* layout, uint32_t attrib);
KB_API uint32_t  kb_vertex_layout_size          (const VertexLayout* layout, uint32_t attrib);

KB_API void      kb_vertex_layout_dump          (const VertexLayout* layout);

#ifdef __cplusplus
}
#endif
