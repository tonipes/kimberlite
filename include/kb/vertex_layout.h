#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "config.h"

typedef enum {
  KB_ATTRIB_FLOAT = 0,
  // TODO: Support more
} AttributeType;

typedef struct {
  uint32_t      location;
  uint32_t      count;
  uint32_t      offset;
  uint32_t      size;
  AttributeType type;
} VertexInputAttribute;

typedef struct {
  uint32_t attrib_count;
  VertexInputAttribute attribs[KB_CONFIG_MAX_VERTEX_ATTRIBS];
} VertexLayout;

uint32_t kb_vertex_layout_attrib_size(AttributeType type, uint32_t num);

void kb_vertex_layout_begin (VertexLayout* layout);
void kb_vertex_layout_add   (VertexLayout* layout, AttributeType type, uint32_t num);
void kb_vertex_layout_end   (VertexLayout* layout);

uint32_t kb_vertex_layout_stride(const VertexLayout* layout);

#ifdef __cplusplus
}
#endif
