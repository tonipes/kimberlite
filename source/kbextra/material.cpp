// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/material.h>

#include <kb/core.h>
#include <kb/rwops.h>
#include <kb/alloc.h>
#include <kb/graphics.h>
#include <kb/rwops.h>

#ifndef KB_TOOL_ONLY

struct kb_material_ref {
  kb_pipeline           pipeline;
  kb_material_uniform   uniforms[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_material_texture   textures[KB_CONFIG_MAX_UNIFORM_BINDINGS];
};

KB_RESOURCE_STORAGE_DEF     (material, kb_material, kb_material_ref, KB_CONFIG_MAX_MATERIALS);
KB_RESOURCE_ALLOC_FUNC_DEF  (material, kb_material, kb_material_create_info, KB_CONFIG_MAX_MATERIALS);
KB_RESOURCE_DATA_HASHED_DEF (material, kb_material);

void kb_material_construct(kb_material handle, const kb_material_create_info info) {
  kb_material_ref& ref = material_ref(handle);
  
  ref.pipeline = info.pipeline;

  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    ref.textures[i] = info.textures[i];
    ref.uniforms[i] = info.uniforms[i];
  }
}

void kb_material_destruct(kb_material handle) { 

}

KB_API void kb_encoder_bind_material(kb_encoder encoder, kb_material material) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(material);
  
  kb_material_ref& ref = material_ref(material);
  
  kb_encoder_bind_pipeline(encoder, ref.pipeline);
  
  kb_shader_binding_slot bind_slot;

  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    kb_material_uniform& uniform = ref.uniforms[i];
    kb_material_texture& texture = ref.textures[i];

    if (uniform.slot_hash > 0) {
      kb_uniform_slot uniform_slot = kb_pipeline_get_uniform_slot(
        ref.pipeline, uniform.slot_hash, KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER, 
        KB_SHADER_STAGE_VERTEX | KB_SHADER_STAGE_FRAGMENT
      );
      kb_encoder_bind_uniform(encoder, uniform_slot, uniform.data, sizeof(uniform.data));
    }
    
    if (texture.slot_hash) {
      kb_uniform_slot texture_slot = kb_pipeline_get_uniform_slot(
        ref.pipeline, texture.slot_hash, KB_SHADER_BINDING_TYPE_TEXTURE, 
        KB_SHADER_STAGE_VERTEX | KB_SHADER_STAGE_FRAGMENT
      );
      kb_encoder_bind_texture(encoder, texture_slot, texture.texture);
    }
    
  }
}
#endif
