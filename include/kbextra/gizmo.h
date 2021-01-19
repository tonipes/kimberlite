// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/foundation.h>
#include <kb/graphics.h>

#include <kbextra/vertex.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(kb_gizmo_handle);

typedef enum kb_gizmo_action {
  KB_GIZMO_ACTION_NONE = 0,
  KB_GIZMO_ACTION_MOVE = 1,
  KB_GIZMO_ACTION_LINE = 2,
} kb_gizmo_action;

typedef struct kb_gizmo_attribs {
  float               offset;
  float               scale;
  kb_float4           color;
  bool                wireframe;
  uint8_t             lod;
} kb_gizmo_attribs;

typedef struct kb_gizmo {
  uint16_t            line_vertex_cache_pos;
  uint16_t            line_index_cache_pos;
  uint32_t            mtx_stack_pos;
  uint32_t            attrib_stack_pos;
  kb_float4x4         view;
  kb_float4x4         proj;
  kb_float3           current_pos;
  kb_gizmo_action     action;
  kb_encoder          encoder;
  kb_pipeline         pipeline;
  kb_simple_vertex*   line_vertex_cache;
  uint16_t*           line_index_cache;
  kb_float4x4         mtx_stack[KB_CONFIG_GIZMO_STACK_SIZE];
  kb_gizmo_attribs    attribs[KB_CONFIG_GIZMO_STACK_SIZE];
} kb_gizmo;

KB_API void     kb_gizmo_create               (kb_gizmo* gizmo);
KB_API void     kb_gizmo_destroy              (kb_gizmo* gizmo);
KB_API void     kb_gizmo_begin                (kb_gizmo* gizmo, kb_encoder encoder, kb_pipeline pipeline);
KB_API void     kb_gizmo_end                  (kb_gizmo* gizmo);
KB_API void     kb_gizmo_push                 (kb_gizmo* gizmo);
KB_API void     kb_gizmo_pop                  (kb_gizmo* gizmo);
KB_API void     kb_gizmo_set_color            (kb_gizmo* gizmo, kb_float4 color);
KB_API void     kb_gizmo_set_lod              (kb_gizmo* gizmo, uint8_t lod);
KB_API void     kb_gizmo_set_wireframe        (kb_gizmo* gizmo, bool wireframe);
KB_API void     kb_gizmo_shape_close          (kb_gizmo* gizmo);
KB_API void     kb_gizmo_shape_move_to        (kb_gizmo* gizmo, const kb_float3 pos);
KB_API void     kb_gizmo_shape_line_to        (kb_gizmo* gizmo, const kb_float3 pos);
KB_API void     kb_gizmo_draw_arc             (kb_gizmo* gizmo, kb_axis axis, const kb_float3 pos, float radius, float degrees);
KB_API void     kb_gizmo_draw_circle          (kb_gizmo* gizmo, const kb_float3 normal, const kb_float3 center, float radius, float weight);
KB_API void     kb_gizmo_draw_cone            (kb_gizmo* gizmo, const kb_float3 from, const kb_float3 to, float radius);
KB_API void     kb_gizmo_draw_cylinder        (kb_gizmo* gizmo, const kb_float3 from, const kb_float3 to, float radius);
KB_API void     kb_gizmo_draw_axis            (kb_gizmo* gizmo, const kb_float3 pos, float length);
KB_API void     kb_gizmo_draw_grid            (kb_gizmo* gizmo, kb_axis axis, const kb_float3 center, uint32_t size, float step);
KB_API void     kb_gizmo_draw_aabb            (kb_gizmo* gizmo, const kb_aabb aabb);
KB_API void     kb_gizmo_flush                (kb_gizmo* gizmo, bool force);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace kb {
  class gizmo: public kb_gizmo {
  public:
    gizmo() {
      kb_gizmo_create(this);
    }

    ~gizmo() {
      kb_gizmo_destroy(this);
    }
  };
};

#endif
