// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <spirv_reflect.h>

uint32_t vertex_attrib_format_count(SpvReflectFormat format) {
  switch (format) {
    case SPV_REFLECT_FORMAT_R32_SFLOAT          : return 1; break;
    case SPV_REFLECT_FORMAT_R32_SINT            : return 1; break;
    case SPV_REFLECT_FORMAT_R32_UINT            : return 1; break;
    case SPV_REFLECT_FORMAT_R32G32_SFLOAT       : return 2; break;
    case SPV_REFLECT_FORMAT_R32G32_SINT         : return 2; break;
    case SPV_REFLECT_FORMAT_R32G32_UINT         : return 2; break;
    case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT    : return 3; break;
    case SPV_REFLECT_FORMAT_R32G32B32_SINT      : return 3; break;
    case SPV_REFLECT_FORMAT_R32G32B32_UINT      : return 3; break;
    case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT : return 4; break;
    case SPV_REFLECT_FORMAT_R32G32B32A32_SINT   : return 4; break;
    case SPV_REFLECT_FORMAT_R32G32B32A32_UINT   : return 4; break;
    default: return 0;
  }
}

kb_vertex_attribute_type vertex_attrib_format_type(SpvReflectFormat format) {
  switch (format) {
    case SPV_REFLECT_FORMAT_R32_SFLOAT          : return KB_ATTRIB_FLOAT; break;
    case SPV_REFLECT_FORMAT_R32_SINT            : return KB_ATTRIB_SINT; break;
    case SPV_REFLECT_FORMAT_R32_UINT            : return KB_ATTRIB_UINT; break;
    case SPV_REFLECT_FORMAT_R32G32_SFLOAT       : return KB_ATTRIB_FLOAT; break;
    case SPV_REFLECT_FORMAT_R32G32_SINT         : return KB_ATTRIB_SINT; break;
    case SPV_REFLECT_FORMAT_R32G32_UINT         : return KB_ATTRIB_UINT; break;
    case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT    : return KB_ATTRIB_FLOAT; break;
    case SPV_REFLECT_FORMAT_R32G32B32_SINT      : return KB_ATTRIB_SINT; break;
    case SPV_REFLECT_FORMAT_R32G32B32_UINT      : return KB_ATTRIB_UINT; break;
    case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT : return KB_ATTRIB_FLOAT; break;
    case SPV_REFLECT_FORMAT_R32G32B32A32_SINT   : return KB_ATTRIB_SINT; break;
    case SPV_REFLECT_FORMAT_R32G32B32A32_UINT   : return KB_ATTRIB_UINT; break;
    default: return KB_ATTRIB_FLOAT;
  }
}

kb_shader_stage get_shader_stage(SpvReflectShaderStageFlagBits type) {
  switch (type) {
    case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT:   return KB_SHADER_STAGE_VERTEX;
    case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT: return KB_SHADER_STAGE_GEOMETRY;
    case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return KB_SHADER_STAGE_FRAGMENT;
    case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT:  return KB_SHADER_STAGE_COMPUTE;

    default: return KB_SHADER_STAGE_UNKNOWN;
  }
}

kb_shader_binding_type get_binding_type(SpvReflectDescriptorType type) {
  switch (type) {
    case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return KB_SHADER_BINDING_TYPE_TEXTURE;
    case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:         return KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER;
    case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:         return KB_SHADER_BINDING_TYPE_STORAGE_BUFFER;
    default: return KB_SHADER_BINDING_TYPE_UNSUPPORTED;
  }
}

void add_vertex_layout(kb_vertex_layout* layout, void* spirv_code, uint32_t code_size) {
  SpvReflectShaderModule reflect;
  spvReflectCreateShaderModule(code_size, spirv_code, &reflect);

  uint32_t attrib_count = 0;
  kb_vertex_attribute_type attrib_types[KB_CONFIG_MAX_VERTEX_ATTRIBS] = {};
  uint32_t attrib_counts [KB_CONFIG_MAX_VERTEX_ATTRIBS] = {};
  uint32_t attrib_locations [KB_CONFIG_MAX_VERTEX_ATTRIBS] = {};
  bool attrib_slots [KB_CONFIG_MAX_VERTEX_ATTRIBS] = {};

  for (uint32_t i = 0; i < reflect.input_variable_count; i++) {
    SpvReflectInterfaceVariable* var = &reflect.input_variables[i];
    bool is_builtin = !(var->built_in < SpvBuiltInMax);

    if (var->location == UINT32_MAX) { kb_log_warn("Invalid vertex attrib location!"); continue; }

    uint32_t idx = var->location;

    attrib_counts     [idx] = vertex_attrib_format_count(var->format);
    attrib_types      [idx] = vertex_attrib_format_type(var->format);
    attrib_locations  [idx] = var->location;
    attrib_slots      [idx] = true;
  }

  kb_vertex_layout_begin(layout);
  for (uint32_t i = 0; i < reflect.input_variable_count; i++) {
    if (!attrib_slots[i]) continue;
    kb_vertex_layout_add(layout, attrib_types[i], attrib_counts[i]);
  }
  kb_vertex_layout_end(layout);

  spvReflectDestroyShaderModule(&reflect);
}

uint32_t get_combined_index(uint32_t set, uint32_t binding) {
  return (set << 16) | (binding & 0x0000FFFF);
}

uint32_t get_binding(uint32_t index) {
  return (index & 0x0000FFFF);
}

uint32_t get_set(uint32_t index) {
  return (index >> 16);
}

void add_binding(kb_shader_desc* bindings, SpvReflectDescriptorBinding* arg, kb_shader_stage stage) {
  const char*             name  = arg->name;
  uint32_t                index = get_combined_index(arg->set, arg->binding);
  kb_shader_binding_type  type  = get_binding_type(arg->descriptor_type);

  switch (type) {
    case KB_SHADER_BINDING_TYPE_TEXTURE: {
      kb_shader_texture_binding* binding = kb_shader_desc_get_texture_binding(bindings, name, index);
      binding->name   = kb_strdup(name);
      binding->index  = index;
      binding->stages = (kb_shader_stage) ((uint32_t) binding->stages | (uint32_t) stage);
    } break;
    case KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER: {
      kb_shader_buffer_binding* binding = kb_shader_desc_get_buffer_binding(bindings, name, index);
      binding->name   = kb_strdup(name);
      binding->index  = index;
      binding->size   = arg->block.size;
      binding->stages = (kb_shader_stage) ((uint32_t) binding->stages | (uint32_t) stage);
    } break;
    default: break;
  }
}

KB_INTERNAL void add_shader_description(kb_shader_desc* bindings, void* spirv_code, uint32_t code_size) {
  SpvReflectShaderModule reflect;
  spvReflectCreateShaderModule(code_size, spirv_code, &reflect); 
  
  for (uint32_t i = 0; i < reflect.descriptor_binding_count; i++) {
    add_binding(bindings, &reflect.descriptor_bindings[i], get_shader_stage(reflect.shader_stage));
  }

  spvReflectDestroyShaderModule(&reflect);
}
