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

typedef struct kb_material_create_info {
  kb_pipeline         pipeline;
  uint32_t            uniform_count;
  kb_uniform_binding* uniforms;
  uint32_t            sampler_count;
  kb_sampler_binding* samplers;
} kb_material_create_info;

KB_RESOURCE_HASHED_FUNC_DECLS (material, kb_material, kb_material_create_info)
KB_RESOURCE_CORE_FUNC_DECLS   (material, kb_material, kb_material_create_info)

KB_API void kb_encoder_bind_material(kb_encoder encoder, kb_material material);

#ifdef __cplusplus
}
#endif
