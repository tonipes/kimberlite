// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>
#include <kb/table.h>
#include <kb/resource.h>
#include <kb/graphics.h>

// KB_HANDLE(kb_material);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_material      { kb_handle_idx idx; } kb_material;


typedef struct kb_material_texture {
  kb_hash                   slot_hash;
  kb_texture                texture;
} kb_material_texture;

typedef struct kb_material_uniform {
  kb_hash                   slot_hash;
  float                     data[KB_CONFIG_MAX_UNIFORM_BLOCK_SIZE];
} kb_material_uniform;

typedef struct kb_material_create_info {
  kb_pipeline               pipeline;
  kb_material_uniform       uniforms[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_material_texture       textures[KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_material_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (material, kb_material, kb_material_create_info)
KB_RESOURCE_ALLOC_FUNC_DECLS  (material, kb_material, kb_material_create_info)

KB_API void kb_encoder_bind_material(kb_encoder encoder, kb_material material);

#ifdef __cplusplus
}
#endif
