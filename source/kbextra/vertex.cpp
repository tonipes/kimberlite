// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/vertex.h>

#include <kb/crt.h>
#include <kb/log.h>

const kb_vertex_layout_info kb_simple_vertex_layout_info = {
  .attribs = {
    { .buffer = 0, .format = KB_VERTEX_FORMAT_FLOAT4 }, // Position
    { .buffer = 0, .format = KB_VERTEX_FORMAT_FLOAT4 }, // Color
    { .buffer = 0, .format = KB_VERTEX_FORMAT_FLOAT4 }, // Texcord
  },
  .buffers = {
    { .step_rate = 1, .step_func = KB_STEP_FUNC_VERTEX, .stride = sizeof(kb_simple_vertex) },
  }
};

KB_INTERNAL uint64_t cv_attrib_sizes(kb_vertex_attribute_type type) {
  switch (type) {
    case KB_ATTRIB_FLOAT:   return 4;
    case KB_ATTRIB_SINT:    return 4;
    case KB_ATTRIB_UINT:    return 4;
    default:                return 0;
  }
}
void kb_vertex_layout_begin(kb_vertex_layout* layout) {
  KB_ASSERT_NOT_NULL(layout);

  layout->attrib_count = 0;
}

uint32_t kb_vertex_layout_add(kb_vertex_layout* layout, kb_vertex_attribute_type type, uint32_t count) {
  KB_ASSERT_NOT_NULL(layout);

  uint32_t i = layout->attrib_count++;

  layout->attribs[i] = {};  
  layout->attribs[i].type       = type;
  layout->attribs[i].count      = count;
  layout->attribs[i].size       = kb_vertex_layout_attrib_size(type, count);
  layout->attribs[i].location   = i;
  
  for (uint32_t j = 0; j < i; j++) {
    layout->attribs[i].offset += layout->attribs[j].size;
  }

  return i;
}

uint32_t kb_vertex_layout_attrib_size(kb_vertex_attribute_type type, uint32_t count) {
  return cv_attrib_sizes(type) * count;
}

void kb_vertex_layout_end(kb_vertex_layout* layout) {
  KB_ASSERT_NOT_NULL(layout);

  kb_hash_gen gen;

  kb_hash_begin(&gen);
  for (uint32_t i = 0; i < layout->attrib_count; i++) {
    kb_hash_add(&gen, layout->attribs[i].count);
    kb_hash_add(&gen, layout->attribs[i].location);
    kb_hash_add(&gen, layout->attribs[i].offset);
    kb_hash_add(&gen, layout->attribs[i].size);
    kb_hash_add(&gen, layout->attribs[i].type);
  }

  layout->hash = kb_hash_end(&gen);
}

uint32_t kb_vertex_layout_stride(const kb_vertex_layout* layout) {
  KB_ASSERT_NOT_NULL(layout);

  uint32_t stride = 0;
  for (uint32_t i = 0; i < layout->attrib_count; i++) {
    stride += layout->attribs[i].size;
  }
  return stride;
}

uint32_t kb_vertex_layout_offset(const kb_vertex_layout* layout, uint32_t attrib) {
  KB_ASSERT_NOT_NULL(layout);

  if (attrib >= layout->attrib_count) return 0;
  return layout->attribs[attrib].offset;
}

uint32_t kb_vertex_layout_size(const kb_vertex_layout* layout, uint32_t attrib) {
  KB_ASSERT_NOT_NULL(layout);

  if (attrib >= layout->attrib_count) return 0;
  return layout->attribs[attrib].size;
}

void kb_vertex_layout_dump(const kb_vertex_layout* layout) {
  KB_ASSERT_NOT_NULL(layout);

  kb_log_debug("Vertex Layout (Stride: {}):", kb_vertex_layout_stride(layout));
  for (uint32_t i = 0; i < layout->attrib_count; i++) {
    auto& attrib = layout->attribs[i];
    kb_log_debug("\tlocation: {}, size: {}, offset: {}", attrib.location, attrib.size, attrib.offset);
  }
}

KB_API bool kb_vertex_layout_match(const kb_vertex_layout* a, const kb_vertex_layout* b) {
  KB_ASSERT_NOT_NULL(a);
  KB_ASSERT_NOT_NULL(b);

  return a->hash == b->hash;
}
