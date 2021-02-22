// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "foundation/alloc.h"
#include "foundation/hash.h"
#include "foundation/resource.h"
#include "foundation/handle.h"
#include "foundation/math.h"
#include "foundation/stream.h"

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(kb_buffer);
KB_HANDLE(kb_pipeline);
KB_HANDLE(kb_encoder);
KB_HANDLE(kb_texture);
KB_HANDLE(kb_graphics_pipe);

typedef enum kb_attachment_flags {
  KB_ATTACHMENT_FLAGS_NONE              = 0 << 0,
  KB_ATTACHMENT_FLAGS_SURFACE_PROXY     = 1 << 0,
  KB_ATTACHMENT_FLAGS_SIZE_IS_RELATIVE  = 1 << 1,
  KB_ATTACHMENT_FLAGS_USE_SURFACE_SIZE  = 1 << 2,
} kb_attachment_flags;

typedef enum kb_submit_call_type {
  KB_SUBMIT_CALL_UNKNOWN          = 0,
  KB_SUBMIT_CALL_DRAW             = 1,
  KB_SUBMIT_CALL_COMPUTE          = 2,
} kb_submit_call_type;

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

typedef enum kb_blend_op {
  KB_BLEND_OP_UNKNOWN             = 0,
  KB_BLEND_OP_ADD                 = 1,
  KB_BLEND_OP_SUBTRACT            = 2,
  KB_BLEND_OP_REVERSE_SUBTRACT    = 3,
  KB_BLEND_OP_MIN                 = 4,
  KB_BLEND_OP_MAX                 = 5,
} kb_blend_op;

typedef enum kb_blend_factor {
  KB_BLEND_UNKNOWN                = 0,
  KB_BLEND_ZERO                   = 1,
  KB_BLEND_ONE                    = 2,
  KB_BLEND_SRC_COLOR              = 3,
  KB_BLEND_ONE_MINUS_SRC_COLOR    = 4,
  KB_BLEND_SRC_ALPHA              = 5,
  KB_BLEND_ONE_MINUS_SRC_ALPHA    = 6,
  KB_BLEND_DST_COLOR              = 7,
  KB_BLEND_ONE_MINUS_DST_COLOR    = 8,
  KB_BLEND_DST_ALPHA              = 9,
  KB_BLEND_ONE_MINUS_DST_ALPHA    = 10,
  KB_BLEND_SRC_ALPHA_SATURATED    = 11,
  KB_BLEND_BLEND_COLOR            = 12,
  KB_BLEND_ONE_MINUS_BLEND_COLOR  = 13,
  KB_BLEND_BLEND_ALPHA            = 14,
  KB_BLEND_ONE_MINUS_BLEND_ALPHA  = 15,
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
  KB_TOPOLOGY_UNKNOWN             = 0,
  KB_TOPOLOGY_TRIANGLE_STRIP      = 1,
  KB_TOPOLOGY_TRIANGLE_LIST       = 2,
  KB_TOPOLOGY_LINE_STRIP          = 3,
  KB_TOPOLOGY_LINE_LIST           = 4,
  KB_TOPOLOGY_POINT_LIST          = 5,
} kb_topology_type;

typedef enum kb_binding_type {
  KB_BINDING_TYPE_UNKNOWN         = 0,
  KB_BINDING_TYPE_TEXTURE         = 1,
  KB_BINDING_TYPE_UNIFORM_BUFFER  = 2,
  KB_BINDING_TYPE_STORAGE_BUFFER  = 3,
} kb_binding_type;

typedef enum kb_format {
  KB_FORMAT_UNSUPPORTED           = 0,
  KB_FORMAT_R8_UNORM              = 1,
  KB_FORMAT_R8_SNORM              = 2,
  KB_FORMAT_R8_UINT               = 3,
  KB_FORMAT_R8_SINT               = 4,
  KB_FORMAT_R16_UNORM             = 5,
  KB_FORMAT_R16_SNORM             = 6,
  KB_FORMAT_R16_UINT              = 7,
  KB_FORMAT_R16_SINT              = 8,
  KB_FORMAT_R16_FLOAT             = 9,
  KB_FORMAT_RG8_UNORM             = 10,
  KB_FORMAT_RG8_SNORM             = 11,
  KB_FORMAT_RG8_UINT              = 12,
  KB_FORMAT_RG8_SINT              = 13,
  KB_FORMAT_R32_UINT              = 14,
  KB_FORMAT_R32_SINT              = 15,
  KB_FORMAT_R32_FLOAT             = 16,
  KB_FORMAT_RG16_UNORM            = 17,
  KB_FORMAT_RG16_SNORM            = 18,
  KB_FORMAT_RG16_UINT             = 19,
  KB_FORMAT_RG16_SINT             = 20,
  KB_FORMAT_RG16_FLOAT            = 21,
  KB_FORMAT_RGBA8_UNORM           = 22,
  KB_FORMAT_RGBA8_SNORM           = 23,
  KB_FORMAT_RGBA8_UINT            = 24,
  KB_FORMAT_RGBA8_SINT            = 25,
  KB_FORMAT_RG32_UINT             = 26,
  KB_FORMAT_RG32_SINT             = 27,
  KB_FORMAT_RG32_FLOAT            = 28,
  KB_FORMAT_RGBA16_UNORM          = 29,
  KB_FORMAT_RGBA16_SNORM          = 30,
  KB_FORMAT_RGBA16_UINT           = 31,
  KB_FORMAT_RGBA16_SINT           = 32,
  KB_FORMAT_RGBA16_FLOAT          = 33,
  KB_FORMAT_RGBA32_UINT           = 34,
  KB_FORMAT_RGBA32_SINT           = 35,
  KB_FORMAT_RGBA32_FLOAT          = 36,

  KB_FORMAT_SURFACE               = 90,
  KB_FORMAT_DEPTH                 = 91,
  KB_FORMAT_STENCIL               = 92,
  KB_FORMAT_DEPTH_STENCIL         = 93,
} kb_format;

typedef enum kb_filter {
  KB_FILTER_NEAREST               = 0,
  KB_FILTER_LINEAR                = 1,
} kb_filter;

typedef enum kb_buffer_usage {
  KB_BUFFER_USAGE_UNKNOWN         = 0,
  KB_BUFFER_USAGE_VERTEX_BUFFER   = 1 << 1,
  KB_BUFFER_USAGE_INDEX_BUFFER    = 1 << 2,
  KB_BUFFER_USAGE_UNIFORM_BUFFER  = 1 << 3,
  KB_BUFFER_USAGE_CPU_WRITE       = 1 << 4,
  KB_BUFFER_USAGE_CPU_READ        = 1 << 5,
} kb_buffer_usage;

typedef enum kb_texture_usage {
  KB_TEXTURE_USAGE_UNKNOWN        = 0,
  KB_TEXTURE_USAGE_RENDER_TARGET  = 1 << 1,
  KB_TEXTURE_USAGE_SHADER_WRITE   = 1 << 2,
  KB_TEXTURE_USAGE_SHADER_READ    = 1 << 3,
} kb_texture_usage;
  
typedef enum kb_shader_stage {
  KB_SHADER_STAGE_UNKNOWN         = 0,
  KB_SHADER_STAGE_VERTEX          = 1 << 0,
  KB_SHADER_STAGE_GEOMETRY        = 1 << 1,
  KB_SHADER_STAGE_FRAGMENT        = 1 << 2,
  KB_SHADER_STAGE_COMPUTE         = 1 << 3,
} kb_shader_stage;

typedef enum kb_load_action {
  KB_LOAD_ACTION_DONT_CARE        = 0,
  KB_LOAD_ACTION_LOAD             = 1,
  KB_LOAD_ACTION_CLEAR            = 2,
} kb_load_action;
  
typedef enum kb_store_action {
  KB_STORE_ACTION_DONT_CARE       = 0,
  KB_STORE_ACTION_STORE           = 1,
} kb_store_action;

typedef enum kb_sampler_address_mode {
  KB_SAMPLER_ADDRESS_MODE_UNKNOWN               = 0,
  KB_SAMPLER_ADDRESS_MODE_REPEAT                = 1,
  KB_SAMPLER_ADDRESS_MODE_MIRROR_REPEAT         = 2,
  KB_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE         = 3,
  KB_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE  = 4,
} kb_sampler_address_mode;

typedef struct kb_buffer_memory {
  kb_buffer                 buffer;
  uint64_t                  offset;
} kb_buffer_memory;

typedef struct kb_blend_info {
  bool                      enabled;
  kb_blend_op               alpha_blend_op;
  kb_blend_op               color_blend_op;
  kb_blend_factor           dst_alpha_blend_factor;
  kb_blend_factor           dst_color_blend_factor;
  kb_blend_factor           src_alpha_blend_factor;
  kb_blend_factor           src_color_blend_factor;
} kb_blend_info;

typedef struct kb_color_blending_info {
  kb_blend_info             attachments[KB_CONFIG_MAX_PASS_COLOR_ATTACHMENTS];
} kb_color_blending_info;
  
typedef struct kb_texture_info {
  uint32_t                  width;
  uint32_t                  height;
  kb_format                 format;
} kb_texture_info;

typedef struct kb_texture_data {
  kb_texture_info           header;
  uint64_t                  data_size;
  void*                     data;
} kb_texture_data;

typedef struct kb_uniform_buffer_info {
  kb_hash                   hash;
  uint32_t                  slot;
} kb_uniform_buffer_info;

typedef struct kb_uniform_texture_info {
  kb_hash                   hash;
  uint32_t                  slot;
} kb_uniform_texture_info;

typedef struct kb_uniform_layout {
  kb_uniform_buffer_info    vertex_uniforms   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_buffer_info    fragment_uniforms [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_buffer_info    compute_uniforms  [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_texture_info   vertex_textures   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_texture_info   fragment_textures [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_texture_info   compute_textures  [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_uniform_layout;

typedef struct kb_sampler_info {
  kb_filter                 min_filter;
  kb_filter                 mag_filter;
  kb_sampler_address_mode   address_mode_u;
  kb_sampler_address_mode   address_mode_v;
  kb_sampler_address_mode   address_mode_w;
  float                     anisotropy;
} kb_sampler_info;

typedef struct kb_texture_create_info {
  kb_stream*                rwops;
  kb_texture_info           texture;
  kb_sampler_info           sampler;
  kb_texture_usage          usage;
  bool                      mipmaps;
  const char*               debug_label;
  void*                     userdata;
} kb_texture_create_info;

typedef struct kb_attachment_info {
  kb_format                 format;
  kb_float2                 size;
  kb_attachment_flags       flags;
  kb_sampler_info           sampler;
  kb_texture_usage          usage;

} kb_attachment_info;

typedef struct kb_attachment_bind_info {
  kb_float4                 clear_color;
  float                     clear_depth;
  uint32_t                  clear_stencil;
  kb_load_action            load_action;
  kb_store_action           store_action;
} kb_attachment_bind_info;

typedef struct kb_attachment_binding {
  uint32_t                  attachment;
  kb_attachment_bind_info   info;
  bool                      active;
} kb_attachment_binding;
  
typedef struct kb_pass_info {
  kb_attachment_binding     color_attachments[KB_CONFIG_MAX_PASS_COLOR_ATTACHMENTS];
  kb_attachment_binding     depth_attachment;
  kb_attachment_binding     stencil_attachment;
} kb_pass_info;

typedef struct kb_graphics_pipeline_info {
  kb_attachment_info        attachments[KB_CONFIG_MAX_PIPE_ATTACHMENTS];
  kb_pass_info              passes[KB_CONFIG_MAX_PASSES];
  int32_t                   attachment_count;
  int32_t                   pass_count;
} kb_graphics_pipeline_info;

typedef struct kb_buffer_create_info {
  kb_stream*                rwops;
  uint32_t                  size;
  kb_buffer_usage           usage;
  const char*               debug_label;
  void*                     userdata;
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
  kb_stream*                rwops;
  const char*               entry;
} kb_shader_info;

typedef struct kb_rasterizer_info {
  kb_cull_mode              cull_mode;
  kb_winding                winding;
  kb_polygon_mode           polygon_mode;
} kb_rasterizer_info;

typedef struct kb_depth_stencil_info {
  bool                      depth_test;
  bool                      depth_write;
  bool                      stencil_test;
} kb_depth_stencil_info;

typedef struct kb_pipeline_render_info {
  kb_topology_type          topology;
  kb_vertex_layout_info     vertex_layout;
  kb_rasterizer_info        rasterizer;
  kb_depth_stencil_info     depth_stencil;
  uint32_t                  samples;
  kb_color_blending_info    color_blending;
  bool                      alpha_to_coverage;
} kb_pipeline_render_info;

typedef struct kb_pipeline_compute_info {

} kb_pipeline_compute_info;

typedef struct kb_pipeline_create_info {
  kb_pipeline_render_info   render_info;
  kb_pipeline_compute_info  compute_info;
  
  kb_uniform_layout         uniform_layout;
  kb_shader_info            vertex_shader;
  kb_shader_info            fragment_shader;
  kb_shader_info            compute_shader;
  uint32_t                  pass;
  kb_shader_stage           stages;
  const char*               debug_label;
  void*                     userdata;
} kb_pipeline_create_info;

typedef struct kb_graphics_init_info {
  bool                      vsync;
  kb_int2                   resolution;
  kb_graphics_pipeline_info pipe;
} kb_graphics_init_info;

typedef struct kb_primitive_info {
  uint32_t                  first_index;
  uint32_t                  first_vertex;
  uint32_t                  index_count;
} kb_primitive_info;

typedef struct kb_vertex_buffer_binding {
  kb_buffer_memory          memory;
} kb_vertex_buffer_binding;

typedef struct kb_index_buffer_binding {
  kb_buffer_memory          memory;
  kb_index_type             index_type;
} kb_index_buffer_binding;

typedef struct kb_texture_binding {
  kb_texture                texture;
} kb_texture_binding;

typedef struct kb_uniform_binding {
  kb_buffer_memory          memory;
} kb_uniform_binding;

typedef struct kb_render_call {
  kb_pipeline               pipeline;
  uint32_t                  first_vertex;
  uint32_t                  first_index;
  uint32_t                  index_count;
  uint32_t                  instance_count;
  kb_index_buffer_binding   index_buffer;
  kb_vertex_buffer_binding  vertex_buffer_bindings    [KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS];  
  kb_texture_binding        vertex_texture_bindings   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        vertex_uniform_bindings   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_texture_binding        fragment_texture_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        fragment_uniform_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_render_call;

typedef struct kb_compute_call {
  kb_pipeline               pipeline;
  kb_int3                   groups;
  kb_int3                   group_size;
  kb_texture_binding        texture_bindings   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        uniform_bindings   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_compute_call;

typedef struct kb_uniform_slot {
  uint32_t                  vertex_slot;
  uint32_t                  fragment_slot;
  uint32_t                  compute_slot;
  kb_shader_stage           stage;
  kb_binding_type           type;
} kb_uniform_slot;

typedef struct kb_graphics_stats {
  float                          platform_frametime;
  float                          platform_frametime_avg;
  float                          platform_frametime_min;
  float                          platform_frametime_max;
  
  float                          frametime;
  float                          frametime_avg;
  float                          frametime_min;
  float                          frametime_max;
  
  uint32_t                       buffer_count;
  uint32_t                       texture_count;
  uint32_t                       pipeline_count;
  
  uint32_t                       encoders_allocated;
  uint32_t                       encoders_used;
  
  uint32_t                       draw_calls_allocated;
  uint32_t                       draw_calls_used;
  
  uint32_t                       compute_calls_allocated;
  uint32_t                       compute_calls_used;
  
  uint32_t                       transient_allocated;
  uint32_t                       transient_used;
} kb_graphics_stats;

KB_RESOURCE_HASHED_FUNC_DECLS (buffer         , kb_buffer         , kb_buffer_create_info         )
KB_RESOURCE_HASHED_FUNC_DECLS (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , kb_texture        , kb_texture_create_info        )

KB_RESOURCE_ALLOC_FUNC_DECLS  (buffer         , kb_buffer         , kb_buffer_create_info         )
KB_RESOURCE_ALLOC_FUNC_DECLS  (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_ALLOC_FUNC_DECLS  (texture        , kb_texture        , kb_texture_create_info        )

KB_API void                 kb_graphics_init                          (const kb_graphics_init_info info);
KB_API void                 kb_graphics_deinit                        (void);
KB_API void                 kb_graphics_frame                         (void);
KB_API void                 kb_graphics_get_stats                     (kb_graphics_stats* dst);
KB_API void                 kb_graphics_run_encoders                  (void);
KB_API kb_int2              kb_graphics_get_extent                    (void);
KB_API float                kb_graphics_get_aspect                    (void);
KB_API uint32_t             kb_graphics_get_current_resource_slot     (void);
KB_API void*                kb_graphics_get_buffer_mapped             (kb_buffer_memory memory);
KB_API void                 kb_graphics_memory_write                  (const void* src, uint64_t size, kb_buffer_memory memory);
KB_API kb_buffer_memory     kb_graphics_transient_alloc               (uint64_t size, kb_buffer_usage usage);
KB_API kb_buffer_memory     kb_graphics_transient_write               (const void* src, uint64_t size, kb_buffer_usage usage);
KB_API kb_texture           kb_graphics_pipe_attachment_texture       (uint32_t attachment);
KB_API kb_format            kb_graphics_pipe_attachment_format        (uint32_t attachment);
KB_API bool                 kb_graphics_pipe_attachment_surface_proxy (uint32_t attachment);
KB_API kb_pass_info*        kb_graphics_get_pass_info                 (uint32_t pass);
KB_API kb_uniform_slot      kb_uniform_get_slot                       (const kb_uniform_layout* layout, kb_hash hash, kb_binding_type type);
KB_API kb_encoder           kb_encoder_begin                          (void);
KB_API void                 kb_encoder_end                            (kb_encoder encoder);
KB_API void                 kb_encoder_push                           (kb_encoder encoder);
KB_API void                 kb_encoder_pop                            (kb_encoder encoder);
KB_API void                 kb_encoder_bind_pipeline                  (kb_encoder encoder, kb_pipeline pipeline);
KB_API void                 kb_encoder_bind_vertex_buffer             (kb_encoder encoder, uint32_t slot, kb_buffer_memory memory);
KB_API void                 kb_encoder_bind_index_buffer              (kb_encoder encoder, kb_index_type type, kb_buffer_memory memory);
KB_API void                 kb_encoder_bind_texture                   (kb_encoder encoder, const kb_uniform_slot slot, kb_texture texture);
KB_API void                 kb_encoder_bind_uniform                   (kb_encoder encoder, const kb_uniform_slot slot, kb_buffer_memory memory);
KB_API void                 kb_encoder_submit_draw                    (kb_encoder encoder, uint32_t first_vertex, uint32_t first_index, uint32_t index_count, uint32_t instance_count);
KB_API void                 kb_encoder_submit_compute                 (kb_encoder encoder, kb_int3 group_size, kb_int3 groups);

//KB_API void                 kb_encoder_submit_texture_blit            (kb_encoder encoder, kb_int3 group_size, kb_int3 groups);
//KB_API void                 kb_encoder_submit_buffer_blit             (kb_encoder encoder, kb_int3 group_size, kb_int3 groups);

KB_API void                 kb_encoder_reset_frame                    (kb_encoder encoder);
KB_API void                 kb_texture_read                           (kb_texture_data* dst, kb_stream* src);
KB_API void                 kb_texture_write                          (const kb_texture_data* src, kb_stream* dst);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

KB_ENUM_FLAG_OPERATORS(kb_shader_stage);
//KB_ENUM_FLAG_OPERATORS(kb_buffer_usage);
//KB_ENUM_FLAG_OPERATORS(kb_texture_usage);

#endif
//
//typedef enum kb_graphics_platform_call_type {
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_UNKNOWN              = 0,
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_BIND_PIPELINE        = 1,
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_BIND_TEXTURE         = 2,
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_BIND_UNIFORM_BUFFER  = 3,
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_BIND_VERTEX_BUFFER   = 4,
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_BIND_INDEX_BUFFER    = 5,
//  KB_PLATFORM_GRAPHICS_CALL_TYPE_DRAW                 = 6,
//} kb_binding_type;
//
//typedef struct kb_graphics_platform_call {
//  kb_graphics_platform_call_type type;
//
//  union {
//    kb_pipeline         pipeline;
//
//    struct {
//      kb_texture        texture;
//      kb_shader_stages  stages;
//    } texture_bind;
//
//    struct {
//      kb_buffer         buffer;
//      uint32_t          index;
//      uint64_t          offset;
//      uint32_t          index_size;
//    } buffer_bind;
//
//    struct {
//      uint32_t          first_vertex;
//      uint32_t          first_index;
//      uint32_t          index_count;
//      uint32_t          instance_count;
//    } draw_call;
//  };
//} kb_graphics_platform_call;
