#pragma once

#include <kb/core.h>
#include <kb/table.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_shader_binding_type {
  KB_SHADER_BINDING_TYPE_UNKNOWN        = 0,
  KB_SHADER_BINDING_TYPE_TEXTURE        = 1,
  KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER = 6,
  KB_SHADER_BINDING_TYPE_STORAGE_BUFFER = 7,
  KB_SHADER_BINDING_TYPE_UNSUPPORTED    = 0x7FFFFFFF,
} kb_shader_binding_type;

typedef enum kb_shader_stage {
  KB_SHADER_STAGE_UNKNOWN               = 0x00000000,
  KB_SHADER_STAGE_VERTEX                = 0x00000001,
  KB_SHADER_STAGE_GEOMETRY              = 0x00000008,
  KB_SHADER_STAGE_FRAGMENT              = 0x00000010,
  KB_SHADER_STAGE_COMPUTE               = 0x00000020,
} kb_shader_stage;

typedef struct kb_shader_texture_binding {
  const char*     name;
  uint32_t        index;
  uint32_t        sampler_index; // Used with separate sampler
  kb_shader_stage stages;
} kb_shader_texture_binding;

typedef struct kb_shader_buffer_binding {
  const char*     name;
  uint32_t        index;
  uint32_t        size;
  kb_shader_stage stages;
} kb_shader_buffer_binding;

typedef struct kb_shader_desc_mapping {
  kb_table  index_table;
  kb_table  name_table;
} kb_shader_desc_mapping;

typedef struct kb_shader_desc {
  uint32_t                      texture_binding_count;
  kb_shader_texture_binding     texture_bindings[KB_CONFIG_MAX_DESCRIPTOR_BINDINGS];
  
  uint32_t                      buffer_binding_count;
  kb_shader_buffer_binding      buffer_bindings[KB_CONFIG_MAX_DESCRIPTOR_BINDINGS];

  kb_shader_desc_mapping        texture_mapping;
  kb_shader_desc_mapping        buffer_mapping;
} kb_shader_desc;

typedef struct kb_shader_binding_slot {
  kb_shader_binding_type        type;
  uint32_t                      index;
  uint64_t                      size;
} kb_shader_binding_slot;

KB_API void                       kb_shader_desc_begin                (kb_shader_desc* bindings);
KB_API void                       kb_shader_desc_end                  (kb_shader_desc* bindings);
KB_API void                       kb_shader_desc_destroy              (kb_shader_desc* bindings);
KB_API kb_shader_buffer_binding*  kb_shader_desc_get_buffer_binding   (kb_shader_desc* bindings, const char* name, uint32_t index);
KB_API kb_shader_texture_binding* kb_shader_desc_get_texture_binding  (kb_shader_desc* bindings, const char* name, uint32_t index);

KB_API bool                       kb_shader_desc_get_binding_slot     (kb_shader_desc* bindings, const char* name, kb_shader_binding_type type, kb_shader_binding_slot* slot);
KB_API bool                       kb_shader_desc_get_binding_slot_hash(kb_shader_desc* bindings, kb_hash hash, kb_shader_binding_type type, kb_shader_binding_slot* slot);
KB_API void                       kb_shader_desc_dump                 (kb_shader_desc* bindings);

#ifdef __cplusplus
}
#endif
