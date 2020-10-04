#pragma once

#include <kb/core.h>

#include <kb/table.h>
//#include <kb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KB_DESCRIPTOR_TYPE_SAMPLER        = 1,
  KB_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 6,
  KB_DESCRIPTOR_TYPE_STORAGE_BUFFER = 7,
  KB_DESCRIPTOR_TYPE_UNSUPPORTED    = 0x7FFFFFFF,
} kb_descriptor_type;

typedef enum kb_shader_stage {
  KB_SHADER_STAGE_UNSUPPORTED = 0x00000000,
  KB_SHADER_STAGE_VERTEX      = 0x00000001,
  KB_SHADER_STAGE_GEOMETRY    = 0x00000008,
  KB_SHADER_STAGE_FRAGMENT    = 0x00000010,
  KB_SHADER_STAGE_COMPUTE     = 0x00000020,
} kb_shader_stage;

typedef struct {
  const char*     name;
  uint32_t        offset;
  uint32_t        size;
} kb_descriptor_member_desc;

typedef struct {
  const char*               name;
  uint32_t                  set_index;
  uint32_t                  binding_index;
  uint32_t                  size;
  uint32_t                  array_count;
  kb_descriptor_type        type;
  kb_shader_stage           stages;
  uint32_t                  member_count;
  kb_table                  member_name_table;
  kb_descriptor_member_desc members[KB_CONFIG_MAX_BINDING_BLOCK_MEMBERS];

} kb_descriptor_block_desc;

typedef struct {
  uint32_t        index;
  uint32_t        bindings_count;
  uint32_t        bindings_idxs[KB_CONFIG_MAX_DESCRIPTOR_BINDINGS];
} kb_descriptor_set_desc;

typedef struct {
  uint32_t                  descriptor_block_count;
  kb_descriptor_block_desc  descriptor_blocks[KB_CONFIG_MAX_DESCRIPTOR_BINDINGS];
  
  uint32_t                  descriptor_set_count;
  kb_descriptor_set_desc    descriptor_sets[KB_CONFIG_MAX_DESCRIPTOR_SETS];

  kb_table                  descriptor_name_table;
  kb_table                  block_member_name_table;
} kb_shader_desc;

KB_API void kb_shader_desc_begin   (kb_shader_desc* bindings);
KB_API void kb_shader_desc_end     (kb_shader_desc* bindings);
KB_API void kb_shader_desc_destroy (kb_shader_desc* bindings);
KB_API void kb_shader_desc_add     (kb_shader_desc* bindings, void* spirv_code, uint32_t size);
KB_API void kb_shader_desc_dump    (kb_shader_desc* bindings);

KB_API kb_descriptor_member_desc* kb_shader_desc_get_block_member_hash   (kb_shader_desc* bindings, kb_hash hash);
KB_API kb_descriptor_block_desc*  kb_shader_desc_get_descriptor_hash     (kb_shader_desc* bindings, kb_hash hash);

KB_API kb_descriptor_member_desc* kb_shader_desc_get_block_member        (kb_shader_desc* bindings, const char* name);
KB_API kb_descriptor_block_desc*  kb_shader_desc_get_descriptor          (kb_shader_desc* bindings, const char* name);

#ifdef __cplusplus
}
#endif
