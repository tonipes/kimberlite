// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/alloc.h>
#include <kb/hash.h>
#include <kb/resource.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(kb_buffer);
KB_HANDLE(kb_pipeline);
KB_HANDLE(kb_encoder);
KB_HANDLE(kb_renderpass);
KB_HANDLE(kb_texture);

typedef enum kb_compare_func {
  KB_COMPARE_UNKNOWN              = 0,
  KB_COMPARE_NEVER                = 1,
  KB_COMPARE_LESS                 = 2,
  KB_COMPARE_EQUAL                = 3,
  KB_COMPARE_LESS_EQUAL           = 4,
  KB_COMPARE_GREATER              = 5,
  KB_COMPARE_NOT_EQUAL            = 6,
  KB_COMPARE_GREATER_EQUAL        = 7,
  KB_COMPARE_ALWAYS               = 8,
} kb_compare_func;

typedef enum kb_stencil_op {
  KB_STENCIL_UNKNOWN              = 0,
  KB_STENCIL_KEEP                 = 1,
  KB_STENCIL_ZERO                 = 2,
  KB_STENCIL_REPLACE              = 3,
  KB_STENCIL_INCR_CLAMP           = 4,
  KB_STENCIL_DECR_CLAMP           = 5,
  KB_STENCIL_INVERT               = 6,
  KB_STENCIL_INCR_WRAP            = 7,
  KB_STENCIL_DECR_WRAP            = 8,
} kb_stencil_op;

typedef enum kb_blend_factor {
  KB_BLEND_ZERO                   = 0,
  KB_BLEND_ONE                    = 1,
  KB_BLEND_SRC_COLOR              = 2,
  KB_BLEND_ONE_MINUS_SRC_COLOR    = 3,
  KB_BLEND_SRC_ALPHA              = 4,
  KB_BLEND_ONE_MINUS_SRC_ALPHA    = 5,
  KB_BLEND_DST_COLOR              = 6,
  KB_BLEND_ONE_MINUS_DST_COLOR    = 7,
  KB_BLEND_DST_ALPHA              = 8,
  KB_BLEND_ONE_MINUS_DST_ALPHA    = 9,
  KB_BLEND_SRC_ALPHA_SATURATED    = 10,
  KB_BLEND_BLEND_COLOR            = 11,
  KB_BLEND_ONE_MINUS_BLEND_COLOR  = 12,
  KB_BLEND_BLEND_ALPHA            = 13,
  KB_BLEND_ONE_MINUS_BLEND_ALPHA  = 14,
} kb_blend_factor;


typedef enum kb_vertex_attrib_format {
  KB_VERTEX_FORMAT_UNKNOWN        = 0,
  KB_VERTEX_FORMAT_FLOAT          = 1,
  KB_VERTEX_FORMAT_FLOAT2         = 2,
  KB_VERTEX_FORMAT_FLOAT3         = 3,
  KB_VERTEX_FORMAT_FLOAT4         = 4,
} kb_vertex_attrib_format;

typedef enum kb_polygon_mode {
  KB_POLYGON_MODE_UNKNOWN         = 0,
  KB_POLYGON_MODE_FILL            = 1,
  KB_POLYGON_MODE_LINE            = 2,
  KB_POLYGON_MODE_POINT           = 3,
} kb_polygon_mode;

typedef enum kb_winding {
  KB_WINDING_UNKNOWN              = 0,
  KB_WINDING_CW                   = 1,
  KB_WINDING_CCW                  = 2,
} kb_winding;

typedef enum kb_step_func {
  KB_STEP_FUNC_UNKNOWN            = 0,
  KB_STEP_FUNC_VERTEX             = 1,
  KB_STEP_FUNC_INSTANCE           = 2,
} kb_step_func;

typedef enum kb_index_type {
  KB_INDEX_TYPE_16                = 0,
  KB_INDEX_TYPE_32                = 1,
} kb_index_type;

typedef enum kb_cull_mode {
  KB_CULL_NONE                    = 0,
  KB_CULL_BACK                    = 1,
  KB_CULL_FRONT                   = 2,
} kb_cull_mode;

typedef enum kb_topology_type {
  KB_TOPOLOGY_TRIANGLE_STRIP      = 0,
  KB_TOPOLOGY_TRIANGLE_LIST       = 1,
  KB_TOPOLOGY_LINE_STRIP          = 2,
  KB_TOPOLOGY_LINE_LIST           = 3,
  KB_TOPOLOGY_POINT_LIST          = 4,
} kb_topology_type;

typedef enum kb_binding_type {
  KB_BINDING_TYPE_UNKNOWN         = 0,
  KB_BINDING_TYPE_TEXTURE         = 1,
  KB_BINDING_TYPE_UNIFORM_BUFFER  = 6,
  KB_BINDING_TYPE_STORAGE_BUFFER  = 7,
  KB_BINDING_TYPE_UNSUPPORTED     = 0x7FFFFFFF,
} kb_binding_type;

typedef enum kb_shader_stage {
  KB_SHADER_STAGE_UNKNOWN         = 0x00000000,
  KB_SHADER_STAGE_VERTEX          = 0x00000001,
  KB_SHADER_STAGE_GEOMETRY        = 0x00000008,
  KB_SHADER_STAGE_FRAGMENT        = 0x00000010,
  KB_SHADER_STAGE_COMPUTE         = 0x00000020,
} kb_shader_stage;

typedef enum kb_format {
  KB_FORMAT_UNSUPPORTED           = 0,
  KB_FORMAT_R8                    = 1,
  KB_FORMAT_R8G8                  = 2,
  KB_FORMAT_R8G8B8                = 3,
  KB_FORMAT_R8G8B8A8              = 4,
  KB_FORMAT_DEPTH                 = 5,
  KB_FORMAT_DEPTH_STENCIL         = 6,
} kb_format;

typedef enum kb_filter {
  KB_FILTER_NEAREST               = 0,
  KB_FILTER_LINEAR                = 1,
} kb_filter;

typedef struct kb_texture_info {
  uint32_t                width;
  uint32_t                height;
  kb_format               format;
  bool                    render_target;
} kb_texture_info;

typedef struct kb_texture_data {
  kb_texture_info         header;
  uint64_t                data_size;
  void*                   data;
} kb_texture_data;

typedef struct kb_shader_binding_slot {
  kb_binding_type  type;
  uint32_t                index;
  uint64_t                size;
  kb_shader_stage         stages;
} kb_shader_binding_slot;

typedef struct kb_frame_stats {
  uint32_t                encoder_count;
  uint32_t                transient_memory_used;
  uint32_t                draw_calls;
  uint32_t                buffer_count;
  uint32_t                pipeline_count;
  uint32_t                texture_count;
  float                   frametime_avg;
  float                   frametime_min;
  float                   frametime_max;
} kb_frame_stats;

typedef struct kb_uniform_buffer_info {
  uint32_t                slot;
  const char*             name;
  uint32_t                size;
} kb_uniform_buffer_info;

typedef struct kb_uniform_texture_info {
  uint32_t                slot;
  const char*             name;
} kb_uniform_texture_info;

typedef struct kb_uniform_layout {
  kb_uniform_buffer_info    vert_ubos       [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_buffer_info    frag_ubos       [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_texture_info   vert_textures   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_texture_info   frag_textures   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_uniform_layout;

typedef struct kb_texture_create_info {
  kb_rwops*                 rwops;
  kb_texture_info           texture;
  bool                      mipmaps;
  kb_filter                 filter;
} kb_texture_create_info;

typedef struct kb_renderpass_create_info {
  kb_texture                color_attachments[KB_CONFIG_MAX_RENDERPASS_ATTACHMENTS];
  kb_texture                depth_attachment;
  kb_texture                stencil_attachment;
} kb_renderpass_create_info;

typedef struct kb_buffer_create_info {
  kb_rwops*                 rwops;
  uint32_t                  size;
  bool                      host_mapped;
} kb_buffer_create_info;

typedef struct kb_vertex_attrib_info {
  kb_vertex_attrib_format   format;
  uint32_t                  buffer;
  uint32_t                  offset;
} kb_vertex_attrib_info;

typedef struct kb_vertex_buffer_info {
  kb_step_func              step_func;
  uint32_t                  step_rate;
  uint32_t                  stride;
} kb_vertex_buffer_info;

typedef struct kb_vertex_layout_info {
  kb_vertex_attrib_info     attribs[KB_CONFIG_MAX_VERTEX_ATTRIBS];
  kb_vertex_buffer_info     buffers[KB_CONFIG_MAX_VERTEX_ATTRIB_BUFFERS];
} kb_vertex_layout_info;

typedef struct kb_shader_info {
  kb_rwops*                 rwops;
  const char*               entry;
} kb_shader_info;

typedef struct kb_rasterizer_info {
  kb_cull_mode              cull_mode;
  kb_winding                winding;
  kb_polygon_mode           polygon_mode;
} kb_rasterizer_info;

typedef struct kb_sampling_info {
  uint32_t                  samples;
} kb_sampling_info;

typedef struct kb_depth_stencil_info {
  bool                      depth_test;
  bool                      depth_write;
  bool                      stencil_test;
} kb_depth_stencil_info;

typedef struct kb_pipeline_create_info {
  kb_topology_type          topology;
  kb_uniform_layout         uniform_layout;
  kb_shader_info            vert_shader;
  kb_shader_info            frag_shader;
  kb_vertex_layout_info     vertex_layout;
  kb_rasterizer_info        rasterizer;
  kb_depth_stencil_info     depth_stencil;
  kb_sampling_info          sampling;
} kb_pipeline_create_info;

typedef struct kb_graphics_init_info {
  uint32_t                  msaa;
  bool                      vsync;
  Int2                      resolution;
  bool                      hide_cursor;
  void*                     backend_ptr;
} kb_graphics_init_info;

typedef struct kb_primitive_info {
  uint32_t                  first_index;
  uint32_t                  first_vertex;
  uint32_t                  index_count;
} kb_primitive_info;

typedef struct kb_vertex_buffer_binding {
  kb_buffer                 buffer;
  uint64_t                  offset;
  bool                      is_set;
} kb_vertex_buffer_binding;

typedef struct kb_index_buffer_binding {
  kb_buffer                 buffer;
  uint64_t                  offset;
  kb_index_type             index_type;
} kb_index_buffer_binding;

typedef struct kb_texture_binding {
  uint32_t                  index;
  kb_texture                texture;
} kb_texture_binding;

typedef struct kb_uniform_binding {
  uint32_t                  index;
  uint64_t                  size;
  uint64_t                  offset;
} kb_uniform_binding;

typedef struct kb_graphics_call_info {
  uint32_t                  first_vertex;
  uint32_t                  first_index;
  uint32_t                  index_count;
  uint32_t                  instance_count;
} kb_graphics_call_info;

typedef struct kb_graphics_call {
  kb_renderpass             renderpass;
  kb_pipeline               pipeline;
  kb_vertex_buffer_binding  vertex_buffer_bindings[KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS];
  kb_index_buffer_binding   index_buffer;
  kb_texture_binding        vert_texture_bindings[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        vert_uniform_bindings[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_texture_binding        frag_texture_bindings[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        frag_uniform_bindings[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_graphics_call_info     info;
} kb_graphics_call;

typedef struct kb_encoder_frame {
  kb_renderpass             renderpass;
  kb_pipeline               pipeline;
  kb_vertex_buffer_binding  vertex_buffer_bindings[KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS];
  kb_index_buffer_binding   index_buffer;
  kb_texture_binding        vert_texture_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        vert_uniform_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_texture_binding        frag_texture_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        frag_uniform_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_encoder_frame;

typedef struct kb_encoder_state {
  uint32_t                  stack_pos;
  uint32_t                  draw_call_count;
  kb_encoder_frame          stack[KB_CONFIG_GIZMO_STACK_SIZE];  
  kb_graphics_call          draw_calls[KB_CONFIG_MAX_DRAW_CALLS];
} kb_encoder_state;

typedef struct kb_encoder_pool {
  uint16_t                  count;
  kb_encoder_state          states[KB_CONFIG_MAX_ENCODERS];
} kb_encoder_pool;

typedef struct kb_graphics_pass_info {
  int                       unused;
} kb_graphics_pass_info;

typedef struct kb_uniform_slot {
  uint32_t                  vert_index;
  uint32_t                  frag_index;
  kb_shader_stage           stage;
  kb_binding_type    type;
} kb_uniform_slot;

KB_RESOURCE_HASHED_FUNC_DECLS (buffer         , kb_buffer         , kb_buffer_create_info         )
KB_RESOURCE_HASHED_FUNC_DECLS (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , kb_texture        , kb_texture_create_info        )
KB_RESOURCE_HASHED_FUNC_DECLS (renderpass     , kb_renderpass     , kb_renderpass_create_info     )

KB_RESOURCE_ALLOC_FUNC_DECLS  (buffer         , kb_buffer         , kb_buffer_create_info         )
KB_RESOURCE_ALLOC_FUNC_DECLS  (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_ALLOC_FUNC_DECLS  (texture        , kb_texture        , kb_texture_create_info        )
KB_RESOURCE_ALLOC_FUNC_DECLS  (renderpass     , kb_renderpass     , kb_renderpass_create_info     )

KB_API void                       kb_graphics_init                      (const kb_graphics_init_info info);
KB_API void                       kb_graphics_deinit                    ();
KB_API void                       kb_graphics_frame                     ();
KB_API void                       kb_graphics_run_encoders              ();
KB_API Int2                       kb_graphics_get_extent                ();
KB_API Int2                       kb_graphics_get_aspect                ();
KB_API void                       kb_graphics_wait_device_idle          ();
KB_API uint32_t                   kb_graphics_get_current_resource_slot ();
KB_API void                       kb_graphics_get_frame_stats           (kb_frame_stats* stats);
KB_API void*                      kb_graphics_get_buffer_mapped         (kb_buffer buffer);
KB_API void                       kb_graphics_set_renderpass_order      (uint32_t order, kb_renderpass pass);

KB_API void*                      kb_graphics_transient_at              (uint64_t offset);
KB_API uint64_t                   kb_graphics_transient_offset          (void* ptr);
KB_API void*                      kb_graphics_transient_alloc           (uint64_t size, uint64_t align);

KB_API kb_uniform_slot            kb_pipeline_get_uniform_slot          (kb_pipeline pipeline, kb_hash hash, kb_binding_type type, kb_shader_stage stage);

KB_API kb_encoder                 kb_encoder_begin                      ();
KB_API void                       kb_encoder_end                        (kb_encoder encoder);
KB_API void                       kb_encoder_push                       (kb_encoder encoder);
KB_API void                       kb_encoder_pop                        (kb_encoder encoder);
KB_API void                       kb_encoder_bind_renderpass            (kb_encoder encoder, kb_renderpass renderpass);
KB_API void                       kb_encoder_bind_pipeline              (kb_encoder encoder, kb_pipeline pipeline);
KB_API void                       kb_encoder_bind_buffer                (kb_encoder encoder, uint32_t slot, kb_buffer vertex_buffer, uint64_t offset);
KB_API void                       kb_encoder_bind_index_buffer          (kb_encoder encoder, kb_buffer index_buffer, uint64_t offset, kb_index_type type);
KB_API void                       kb_encoder_bind_texture               (kb_encoder encoder, const kb_uniform_slot slot, kb_texture texture);
KB_API void                       kb_encoder_bind_uniform               (kb_encoder encoder, const kb_uniform_slot slot, const void* data, uint64_t size);
KB_API void                       kb_encoder_submit                     (kb_encoder encoder, uint32_t first_vertex, uint32_t first_index, uint32_t index_count, uint32_t instance_count);
KB_API void                       kb_encoder_reset_frame                (kb_encoder encoder);

KB_API void                       kb_texture_read                       (kb_texture_data* dst, kb_rwops* src);
KB_API void                       kb_texture_write                      (const kb_texture_data* src, kb_rwops* dst);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

KB_ENUM_FLAG_OPERATORS(kb_shader_stage);

#endif
