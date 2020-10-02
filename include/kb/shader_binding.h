#pragma once

#include <kb/api.h>
#include <kb/types.h>
#include <kb/table.h>
#include <kb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KB_DESCRIPTOR_TYPE_SAMPLER        = 1,
  KB_DESCRIPTOR_TYPE_UNIFORM_BUFFER = 6,
  KB_DESCRIPTOR_TYPE_STORAGE_BUFFER = 7,
  KB_DESCRIPTOR_TYPE_UNSUPPORTED    = 0x7FFFFFFF,

} DescriptorType;

typedef enum {
  KB_SHADER_STAGE_UNSUPPORTED = 0x00000000,
  KB_SHADER_STAGE_VERTEX      = 0x00000001,
  KB_SHADER_STAGE_GEOMETRY    = 0x00000008,
  KB_SHADER_STAGE_FRAGMENT    = 0x00000010,
  KB_SHADER_STAGE_COMPUTE     = 0x00000020,
} ShaderStage;

typedef struct {
  const char*     name;
  uint32_t        offset;
  uint32_t        size;
} BlockMember;

typedef struct {
  const char*     name;
  uint32_t        set_index;
  uint32_t        binding_index;
  uint32_t        size;
  uint32_t        array_count;
  DescriptorType  type;
  ShaderStage     stages;
  uint32_t        member_count;
  BlockMember     members[KB_CONFIG_MAX_BINDING_BLOCK_MEMBERS];
  Table           member_name_table;
} DescriptorBlock;

typedef struct {
  uint32_t        index;
  uint32_t        bindings_count;
  uint32_t        bindings_idxs[KB_CONFIG_MAX_DESCRIPTOR_BINDINGS];
} DescriptorSet;

typedef struct {
  uint32_t        descriptor_block_count;
  DescriptorBlock descriptor_blocks[KB_CONFIG_MAX_DESCRIPTOR_BINDINGS];
  
  uint32_t        descriptor_set_count;
  DescriptorSet   descriptor_sets[KB_CONFIG_MAX_DESCRIPTOR_SETS];

  Table           descriptor_name_table;
  Table           block_member_name_table;
} ShaderBindings;

KB_API void               kb_shader_binding_begin                   (ShaderBindings* bindings);
KB_API void               kb_shader_binding_end                     (ShaderBindings* bindings);
KB_API void               kb_shader_binding_destroy                 (ShaderBindings* bindings);

KB_API void               kb_shader_binding_add                     (ShaderBindings* bindings, void* spirv_code, uint32_t size);
KB_API void               kb_shader_binding_dump                    (ShaderBindings* bindings);

KB_API BlockMember*       kb_shader_binding_get_block_member_hash   (ShaderBindings* bindings, Hash hash);
KB_API DescriptorBlock*   kb_shader_binding_get_descriptor_hash     (ShaderBindings* bindings, Hash hash);

KB_API BlockMember*       kb_shader_binding_get_block_member        (ShaderBindings* bindings, const char* name);
KB_API DescriptorBlock*   kb_shader_binding_get_descriptor          (ShaderBindings* bindings, const char* name);

#ifdef __cplusplus
}
#endif
