// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/foundation.h>
#include <kb/graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(kb_material);

typedef struct kb_material_texture {
  kb_uniform_slot           slot;
  kb_texture                texture;
} kb_material_texture;

typedef struct kb_material_uniform {
  kb_uniform_slot           slot;
  void*                     data;
  uint64_t                  size;
} kb_material_uniform;

typedef struct kb_material_create_info {
  kb_pipeline               pipeline;
  kb_material_uniform       uniforms[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_material_texture       textures[KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_material_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (material, kb_material, kb_material_create_info);
KB_RESOURCE_ALLOC_FUNC_DECLS  (material, kb_material, kb_material_create_info);

KB_API void kb_encoder_bind_material(kb_encoder encoder, kb_material material);

#ifdef __cplusplus
}
#endif
