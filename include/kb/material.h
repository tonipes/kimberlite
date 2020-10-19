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
#include <kb/renderer.h>

KB_HANDLE(kb_material);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_material_sampler {
  kb_hash                   hash;
  kb_texture                texture;
} kb_material_sampler;

typedef struct kb_material_uniform {
  kb_hash                   hash;
  uint64_t                  size;
  float                     data[128];
} kb_material_uniform;

typedef struct kb_material_create_info {
  uint32_t                  uniform_count;
  uint32_t                  sampler_count;
  kb_pipeline               pipeline;
  kb_material_uniform*      uniforms;
  kb_material_sampler*      samplers;
} kb_material_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (material, kb_material, kb_material_create_info)
KB_RESOURCE_ALLOC_FUNC_DECLS  (material, kb_material, kb_material_create_info)

KB_API void kb_encoder_bind_material(kb_encoder encoder, kb_material material);

#ifdef __cplusplus
}
#endif
