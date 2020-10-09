#pragma once

#include <kb/core.h>
#include <kb/math.h>
#include <kb/graphics.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_gizmo_action {
  KB_GIZMO_ACTION_NONE = 0,
  KB_GIZMO_ACTION_MOVE = 1,
  KB_GIZMO_ACTION_LINE = 2,
} kb_gizmo_action;

typedef struct kb_gizmo_attribs {
  float               offset;
  float               scale;
  Float4              color;
  bool                wireframe;
  uint8_t             lod;
} kb_gizmo_attribs;

typedef struct kb_gizmo {
  uint16_t            line_vertex_cache_pos;
  uint16_t            line_index_cache_pos;
  uint32_t            mtx_stack_pos;
  uint32_t            attrib_stack_pos;
  Float4x4            view;
  Float4x4            proj;
  Float3              current_pos;
  kb_gizmo_action     action;
  kb_encoder          encoder;
  kb_simple_vertex*   line_vertex_cache;
  uint16_t*           line_index_cache;
  Float4x4            mtx_stack[KB_CONFIG_GIZMO_STACK_SIZE];
  kb_gizmo_attribs    attribs[KB_CONFIG_GIZMO_STACK_SIZE];
} kb_gizmo;

KB_API void     kb_gizmo_begin                (kb_gizmo* gizmo, kb_encoder encoder, const Float4x4 view, const Float4x4 proj);
KB_API void     kb_gizmo_end                  (kb_gizmo* gizmo);
KB_API void     kb_gizmo_state_push           (kb_gizmo* gizmo);
KB_API void     kb_gizmo_state_pop            (kb_gizmo* gizmo);
KB_API void     kb_gizmo_state_push_transform (kb_gizmo* gizmo, const Float4x4 mtx, bool flush);
KB_API void     kb_gizmo_state_pop_transform  (kb_gizmo* gizmo, bool flush);
KB_API void     kb_gizmo_state_set_color      (kb_gizmo* gizmo, Float4 color);
KB_API void     kb_gizmo_state_set_lod        (kb_gizmo* gizmo, uint8_t lod);
KB_API void     kb_gizmo_state_set_wireframe  (kb_gizmo* gizmo, bool wireframe);
KB_API void     kb_gizmo_shape_close          (kb_gizmo* gizmo);
KB_API void     kb_gizmo_shape_move_to        (kb_gizmo* gizmo, const Float3 pos);
KB_API void     kb_gizmo_shape_line_to        (kb_gizmo* gizmo, const Float3 pos);
KB_API void     kb_gizmo_draw_arc             (kb_gizmo* gizmo, Axis axis, const Float3 pos, float radius, float degrees);
KB_API void     kb_gizmo_draw_circle          (kb_gizmo* gizmo, const Float3 normal, const Float3 center, float radius, float weight);
KB_API void     kb_gizmo_draw_cone            (kb_gizmo* gizmo, const Float3 from, const Float3 to, float radius);
KB_API void     kb_gizmo_draw_cylinder        (kb_gizmo* gizmo, const Float3 from, const Float3 to, float radius);
KB_API void     kb_gizmo_draw_axis            (kb_gizmo* gizmo, const Float3 pos, float length);
KB_API void     kb_gizmo_draw_grid            (kb_gizmo* gizmo, Axis axis, const Float3 center, uint32_t size, float step);
KB_API void     kb_gizmo_draw_aabb            (kb_gizmo* gizmo, const Aabb aabb);
KB_API void     kb_gizmo_flush                (kb_gizmo* gizmo);
KB_API void     kb_gizmo_flush_quad           (kb_gizmo* gizmo);

#ifdef __cplusplus
}
#endif
