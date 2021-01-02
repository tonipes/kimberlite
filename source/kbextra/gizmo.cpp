// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/gizmo.h>

#include <kb/alloc.h>

static const uint8_t circle_lods[] = { 12 };

inline uint8_t get_circle_lod(uint8_t lod) {
  lod = lod > KB_COUNTOF(circle_lods) - 1 ? KB_COUNTOF(circle_lods) - 1 : lod;
  return circle_lods[lod];
}

inline kb_gizmo_attribs& current_attribs(kb_gizmo* gizmo) {
  KB_ASSERT_NOT_NULL(gizmo);

  return gizmo->attribs[gizmo->attrib_stack_pos];
}

KB_API void kb_gizmo_begin(kb_gizmo* gizmo, kb_encoder encoder, kb_pipeline pipeline) {
  KB_ASSERT_NOT_NULL(gizmo);

  gizmo->pipeline               = pipeline;
  gizmo->encoder                = encoder;
  gizmo->action                 = KB_GIZMO_ACTION_NONE;
  gizmo->attrib_stack_pos       = 0;
  gizmo->mtx_stack_pos          = 0;
  gizmo->line_vertex_cache_pos  = 0;
  gizmo->line_index_cache_pos   = 0;
  gizmo->mtx_stack_pos          = 0;
  gizmo->mtx_stack[0]           = IdentityFloat4x4;
  gizmo->line_vertex_cache      = KB_DEFAULT_ALLOC_TYPE(kb_simple_vertex, (KB_CONFIG_GIZMO_CACHE_SIZE + 1));
  gizmo->line_index_cache       = KB_DEFAULT_ALLOC_TYPE(uint16_t,         (KB_CONFIG_GIZMO_CACHE_SIZE * 2)); // Line has 2 indices

  *gizmo->attribs               = {};
  gizmo->attribs[0].color       = { 1.0f , 1.0f, 1.0f, 1.0f };
}

KB_API void kb_gizmo_end(kb_gizmo* gizmo) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_flush(gizmo, true);

  KB_DEFAULT_FREE(gizmo->line_vertex_cache);
  KB_DEFAULT_FREE(gizmo->line_index_cache);
}

KB_API void kb_gizmo_flush(kb_gizmo* gizmo, bool force) {    
  KB_ASSERT_NOT_NULL(gizmo);
  
  if (!force && gizmo->line_vertex_cache_pos < uint16_t(KB_CONFIG_GIZMO_CACHE_SIZE)) {
    return;
  }

  if (gizmo->line_vertex_cache_pos > 0) {
    uint64_t vertex_alloc_size  = gizmo->line_vertex_cache_pos * sizeof(kb_simple_vertex);
    uint64_t index_alloc_size   = gizmo->line_index_cache_pos  * sizeof(uint16_t);

    kb_simple_vertex* vertex_data_buffer  = (kb_simple_vertex*)   kb_graphics_transient_alloc(vertex_alloc_size,  256);
    uint16_t*         index_data_buffer   = (uint16_t*)           kb_graphics_transient_alloc(index_alloc_size,   256);
    
    uint64_t vertex_data_buffer_offset  = kb_graphics_transient_offset(vertex_data_buffer);
    uint64_t index_data_buffer_offset   = kb_graphics_transient_offset(index_data_buffer);

    if (vertex_data_buffer != NULL) {
      kb_memcpy(vertex_data_buffer, gizmo->line_vertex_cache, vertex_alloc_size);
    }
    
    if (index_data_buffer != NULL) {
      kb_memcpy(index_data_buffer, gizmo->line_index_cache, index_alloc_size);
    }
    
    Float4x4 model = gizmo->mtx_stack[gizmo->mtx_stack_pos];

    kb_encoder_push(gizmo->encoder);
      kb_encoder_bind_pipeline      (gizmo->encoder, gizmo->pipeline);
      kb_encoder_bind_buffer        (gizmo->encoder, 0, {}, vertex_data_buffer_offset);
      kb_encoder_bind_index_buffer  (gizmo->encoder, {}, index_data_buffer_offset, KB_INDEX_TYPE_16);
      kb_encoder_submit             (gizmo->encoder, 0, 0, gizmo->line_index_cache_pos, 1);
    kb_encoder_pop(gizmo->encoder);
  }

  gizmo->action                  = KB_GIZMO_ACTION_NONE;
  gizmo->line_index_cache_pos    = 0;
  gizmo->line_vertex_cache_pos   = 0;
}

KB_API void kb_gizmo_shape_move_to(kb_gizmo* gizmo, const Float3 pos) {
  KB_ASSERT_NOT_NULL(gizmo);

  gizmo->current_pos = pos;
}

KB_API void kb_gizmo_shape_line_to(kb_gizmo* gizmo, const Float3 pos) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_attribs& attribs = current_attribs(gizmo);
  
  kb_gizmo_flush(gizmo, false);
  
  uint32_t start_vertex_index = gizmo->line_vertex_cache_pos++;
  uint32_t end_vertex_index = gizmo->line_vertex_cache_pos++;

  gizmo->line_index_cache[gizmo->line_index_cache_pos++] = start_vertex_index;
  gizmo->line_index_cache[gizmo->line_index_cache_pos++] = end_vertex_index;

  kb_simple_vertex* start_vert = &gizmo->line_vertex_cache[start_vertex_index];
  start_vert->pos     = { gizmo->current_pos.x, gizmo->current_pos.y, gizmo->current_pos.z, 1.0f };
  start_vert->color   = attribs.color;

  kb_simple_vertex* end_vert = &gizmo->line_vertex_cache[end_vertex_index];
  end_vert->pos     = { pos.x, pos.y, pos.z, 1.0f };
  end_vert->color   = attribs.color;

  gizmo->current_pos = pos;
}

KB_API void kb_gizmo_push(kb_gizmo* gizmo) {
  KB_ASSERT_NOT_NULL(gizmo);

  gizmo->attrib_stack_pos++;

  // Inherit old state
  gizmo->attribs[gizmo->attrib_stack_pos] = gizmo->attribs[gizmo->attrib_stack_pos - 1];
}

KB_API void kb_gizmo_pop(kb_gizmo* gizmo) {
  KB_ASSERT_NOT_NULL(gizmo);

  // INCOMPLETE: Check if state has changed in a meaningful way
  if (true) {
    kb_gizmo_flush(gizmo, false);
  }

  --gizmo->attrib_stack_pos;
}

//KB_API void kb_gizmo_push_transform(kb_gizmo* gizmo, const Float4x4 mtx, bool flush) {
//  KB_ASSERT_NOT_NULL(gizmo);
//
//  if (flush) kb_gizmo_flush(gizmo, true);
//
//  Float4x4& stack = gizmo->mtx_stack[gizmo->mtx_stack_pos];
//  gizmo->mtx_stack_pos++;
//
//  gizmo->mtx_stack[gizmo->mtx_stack_pos] = stack * mtx;
//}
//
//KB_API void kb_gizmo_pop_transform(kb_gizmo* gizmo, bool flush) {
//  KB_ASSERT_NOT_NULL(gizmo);
//
//  if (flush) kb_gizmo_flush(gizmo, true);
//  gizmo->mtx_stack_pos--;
//}

KB_API void kb_gizmo_set_color(kb_gizmo* gizmo, Float4 color) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_attribs& attribs = current_attribs(gizmo);
  attribs.color = color; 
}

KB_API void kb_gizmo_draw_circle(kb_gizmo* gizmo, const Float3 normal, const Float3 center, float radius, float weight) {
  KB_ASSERT_NOT_NULL(gizmo);

  const kb_gizmo_attribs& attrib = current_attribs(gizmo);

  const uint32_t num = get_circle_lod(attrib.lod);
  const float step = PI * 2.0f / num;
  
  Float3 udir;
  Float3 vdir;
  tangent_frame_with_spin(normal, 0.0f, &udir, &vdir);

  Float2 xy0 = circle_point(0.0f);
  Float2 xy1 = squircle_point(0.0f);

  Float3 pos  = scale_float3(udir, lerp_scalar(xy0.x, xy1.x, weight) * radius);
  Float3 tmp0 = scale_float3(vdir, lerp_scalar(xy0.y, xy1.y, weight) * radius);
  Float3 tmp1 = add_float3(pos, tmp0);
  Float3 tmp2 = add_float3(tmp1, center);

  Float3 start = tmp2;
  
  kb_gizmo_push(gizmo);

  kb_gizmo_shape_move_to(gizmo, tmp2);

  for (uint32_t i = 1; i < num; ++i) {
    float angle = step * i;
    xy0 = circle_point(angle);
    xy1 = squircle_point(angle);

    pos  = scale_float3(udir, lerp_scalar(xy0.x, xy1.x, weight) * radius);
    tmp0 = scale_float3(vdir, lerp_scalar(xy0.y, xy1.y, weight) * radius);
    tmp1 = add_float3(pos, tmp0);
    tmp2 = add_float3(tmp1, center);

    kb_gizmo_shape_line_to(gizmo, tmp2);
  }

  kb_gizmo_shape_line_to(gizmo, start);
  
  kb_gizmo_pop(gizmo);
}

KB_API void kb_gizmo_draw_arc(kb_gizmo* gizmo, Axis axis, const Float3 pos, float radius, float degrees) {
  KB_ASSERT_NOT_NULL(gizmo);

  const kb_gizmo_attribs& attrib = current_attribs(gizmo);
  
  const uint32_t num = get_circle_lod(attrib.lod);

  auto deg = wrap_scalar(degrees, 360.0f);
  
  Float3 cpos = get_point(axis, sin_scalar(0) * radius, cos_scalar(0) * radius);

  kb_gizmo_shape_move_to(gizmo, add_float3(pos, cpos));
  
  uint32_t n = uint32_t(num * (deg / 360.0f));

  const float step = (2.0f * PI) * (deg / 360.0f) / n;

  for (uint32_t i = 1; i < n + 1; ++i) {
    cpos = get_point(axis, sin_scalar(step * i) * radius, cos_scalar(step * i) * radius);
    kb_gizmo_shape_line_to(gizmo, add_float3(pos, cpos));
  }

  kb_gizmo_shape_move_to(gizmo, pos);
  
  cpos = get_point(axis, sin_scalar(0) * radius, cos_scalar(0) * radius);
  kb_gizmo_shape_line_to(gizmo, add_float3(pos, cpos));

  cpos = get_point(axis, sin_scalar(step * n) * radius, cos_scalar(step * n) * radius);
  kb_gizmo_shape_move_to(gizmo, add_float3(pos, cpos));

  kb_gizmo_shape_line_to(gizmo, pos);
}

KB_API void kb_gizmo_draw_axis(kb_gizmo* gizmo, const Float3 pos, float length) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_push(gizmo);
  {
    kb_gizmo_set_color(gizmo, { 1.0f, 0.0f, 0.0f, 1.0f });
    kb_gizmo_shape_move_to(gizmo, pos);
    kb_gizmo_shape_line_to(gizmo, { pos.x + length, pos.y, pos.z });

    kb_gizmo_set_color(gizmo, { 0.0f, 1.0f, 0.0f, 1.0f });
    kb_gizmo_shape_move_to(gizmo, pos);
    kb_gizmo_shape_line_to(gizmo, { pos.x, pos.y + length, pos.z });

    kb_gizmo_set_color(gizmo, { 0.0f, 0.0f, 1.0f, 1.0f });
    kb_gizmo_shape_move_to(gizmo, pos);
    kb_gizmo_shape_line_to(gizmo, { pos.x, pos.y, pos.z + length });
  }

  kb_gizmo_pop(gizmo);
}

KB_API void kb_gizmo_draw_grid(kb_gizmo* gizmo, Axis axis, const Float3 pos, uint32_t size, float step) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_push(gizmo);
  // kb_gizmo_push_transform(gizmo, transform_mtx(pos), true);
  
  const uint32_t num = (size / 2) * 2;

  const float a = float(size) * step;
  const float b = 0;
  
  float yy = b;
  
  for (uint32_t i = 0; i < num; ++i) {
    kb_gizmo_shape_move_to(gizmo, pos + get_point(axis, b, yy));
    kb_gizmo_shape_line_to(gizmo, pos + get_point(axis, a, yy));
    kb_gizmo_shape_move_to(gizmo, pos + get_point(axis, yy, b));
    kb_gizmo_shape_line_to(gizmo, pos + get_point(axis, yy, a));
    yy += step;
  }

  kb_gizmo_shape_move_to(gizmo, pos + get_point(axis, b, b));
  kb_gizmo_shape_line_to(gizmo, pos + get_point(axis, b, a));
  kb_gizmo_shape_line_to(gizmo, pos + get_point(axis, a, a));
  kb_gizmo_shape_line_to(gizmo, pos + get_point(axis, a, b));

  // kb_gizmo_pop_transform(gizmo, true);
  kb_gizmo_pop(gizmo);
}

KB_API void kb_gizmo_draw_aabb(kb_gizmo* gizmo, const Aabb aabb) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_push(gizmo);

  kb_gizmo_shape_move_to(gizmo, {aabb.min.x, aabb.min.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.max.x, aabb.min.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.max.x, aabb.max.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.min.x, aabb.max.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.min.x, aabb.min.y, aabb.min.z});

  kb_gizmo_shape_move_to(gizmo, {aabb.min.x, aabb.min.y, aabb.max.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.max.x, aabb.min.y, aabb.max.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.max.x, aabb.max.y, aabb.max.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.min.x, aabb.max.y, aabb.max.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.min.x, aabb.min.y, aabb.max.z});

  kb_gizmo_shape_move_to(gizmo, {aabb.min.x, aabb.min.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.min.x, aabb.min.y, aabb.max.z});

  kb_gizmo_shape_move_to(gizmo, {aabb.max.x, aabb.min.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.max.x, aabb.min.y, aabb.max.z});

  kb_gizmo_shape_move_to(gizmo, {aabb.min.x, aabb.max.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.min.x, aabb.max.y, aabb.max.z});

  kb_gizmo_shape_move_to(gizmo, {aabb.max.x, aabb.max.y, aabb.min.z});
  kb_gizmo_shape_line_to(gizmo, {aabb.max.x, aabb.max.y, aabb.max.z});
  
  kb_gizmo_pop(gizmo);
}

KB_API void kb_gizmo_shape_draw_triangle(kb_gizmo* gizmo, const Triangle triangle) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_push(gizmo);
  
  kb_gizmo_shape_move_to(gizmo, triangle.v0);
  kb_gizmo_shape_line_to(gizmo, triangle.v1);
  kb_gizmo_shape_line_to(gizmo, triangle.v2);
  kb_gizmo_shape_line_to(gizmo, triangle.v0);

  kb_gizmo_pop(gizmo);
}

KB_API void kb_gizmo_shape_draw_ray(kb_gizmo* gizmo, const Ray ray) {
  KB_ASSERT_NOT_NULL(gizmo);

  kb_gizmo_push(gizmo);
  
  kb_gizmo_shape_move_to(gizmo, ray.pos);
  kb_gizmo_shape_line_to(gizmo, ray.pos + ray.dir);

  kb_gizmo_pop(gizmo);
}
