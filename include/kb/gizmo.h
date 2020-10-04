#pragma once

#include <kb/core.h>
#include <kb/math.h>
#include <kb/graphics.h>

KB_HANDLE(kb_gizmo);

#ifdef __cplusplus
extern "C" {
#endif

KB_API kb_gizmo kb_gizmo_begin                (kb_command_buffer command_buffer, const Float4x4 view, const Float4x4 proj);
KB_API void     kb_gizmo_end                  (kb_gizmo gizmo);
KB_API void     kb_gizmo_state_push           (kb_gizmo gizmo);
KB_API void     kb_gizmo_state_pop            (kb_gizmo gizmo);
KB_API void     kb_gizmo_state_push_transform (kb_gizmo gizmo, const Float4x4 mtx, bool flush);
KB_API void     kb_gizmo_state_pop_transform  (kb_gizmo gizmo, bool flush);
KB_API void     kb_gizmo_state_set_color      (kb_gizmo gizmo, Float4 color);
KB_API void     kb_gizmo_state_set_lod        (kb_gizmo gizmo, uint8_t lod);
KB_API void     kb_gizmo_state_set_wireframe  (kb_gizmo gizmo, bool wireframe);
KB_API void     kb_gizmo_shape_close          (kb_gizmo gizmo);
KB_API void     kb_gizmo_shape_move_to        (kb_gizmo gizmo, const Float3 pos);
KB_API void     kb_gizmo_shape_line_to        (kb_gizmo gizmo, const Float3 pos);
KB_API void     kb_gizmo_draw_arc             (kb_gizmo gizmo, Axis axis, const Float3 pos, float radius, float degrees);
KB_API void     kb_gizmo_draw_circle          (kb_gizmo gizmo, const Float3 normal, const Float3 center, float radius, float weight);
KB_API void     kb_gizmo_draw_cone            (kb_gizmo gizmo, const Float3 from, const Float3 to, float radius);
KB_API void     kb_gizmo_draw_cylinder        (kb_gizmo gizmo, const Float3 from, const Float3 to, float radius);
KB_API void     kb_gizmo_draw_axis            (kb_gizmo gizmo, const Float3 pos, float length);
KB_API void     kb_gizmo_draw_grid            (kb_gizmo gizmo, Axis axis, const Float3 center, uint32_t size, float step);
KB_API void     kb_gizmo_draw_aabb            (kb_gizmo gizmo, const Aabb aabb);
KB_API void     kb_gizmo_flush                (kb_gizmo gizmo);
KB_API void     kb_gizmo_flush_quad           (kb_gizmo gizmo);

#ifdef __cplusplus
}
#endif
