#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "kb/config.h"
#include "kb/hash.h"
#include "kb/resource.h"
#include "kb/handle.h"
#include "kb/math.h"

//#####################################################################################################################
// Structs
//#####################################################################################################################

KB_HANDLE(BatchHandle);
KB_HANDLE(EncoderHandle);
KB_HANDLE(FramebufferHandle);
KB_HANDLE(GizmoHandle);
KB_HANDLE(IndexBufferHandle);
KB_HANDLE(MaterialHandle);
KB_HANDLE(MeshHandle);
KB_HANDLE(ProgramHandle);
KB_HANDLE(ShaderHandle);  
KB_HANDLE(TextureHandle);
KB_HANDLE(UniformHandle);
KB_HANDLE(UniformBlockHandle);
KB_HANDLE(VertexBufferHandle);
KB_HANDLE(ViewHandle);

//#####################################################################################################################
// Enums
//#####################################################################################################################

typedef enum {
  Single,
  Instanced,
} SubmitType;

typedef enum {
  Sampler,
  End,
  Vec4,
  Mat3,
  Mat4,
  Count
} UniformType;

typedef enum {
  DebugOverlayModeText,
  DebugOverlayModeStats,
  DebugOverlayModeProfiler,
  DebugOverlayModeCount,
} DebugOverlayMode;

//#####################################################################################################################
// Structs
//#####################################################################################################################

// TODO: This is identical to bgfx_vertex_layout_t
// and it really should be just an alias
typedef struct {
  Hash      hash;
  uint16_t  stride;
  uint16_t  offset  [KB_VERTEX_ATTRIB_COUNT];
  uint16_t  attribs [KB_VERTEX_ATTRIB_COUNT];
} VertexLayout;

typedef struct {
  uint32_t count;
  uint32_t stride;
} BatchCreateInfo;

typedef struct {
  ViewHandle view;
  BatchHandle batch;
} BatchRenderInfo;

typedef struct {
  int param;
} EncoderCreateInfo;

typedef struct {
  Int2 size;
} FramebufferCreateInfo;

typedef struct {
  ViewHandle view;
} GizmoCreateInfo;

typedef struct {
  const Memory memory;
} IndexBufferCreateInfo;

typedef struct {
  ProgramHandle program;
  UniformBlockHandle uniforms;
} MaterialCreateInfo;

typedef struct {
  uint32_t first_vertex;
  uint32_t vertex_count;
  VertexBufferHandle vertex_buffer;

  uint32_t first_index;
  uint32_t index_count;
  IndexBufferHandle index_buffer;
} MeshCreateInfo;

typedef struct {
  ShaderHandle vertex_shader;
  ShaderHandle vertex_shader_instanced;
  ShaderHandle fragment_shader;
} ProgramCreateInfo;

typedef struct {
  int param;
} TextureCreateInfo;

typedef struct {
  UniformHandle target;
  TextureHandle texture;
} TextureSetInfo;

typedef struct {
  const char* name;
  UniformType type;
  uint16_t count;
  uint16_t sampler;
} UniformCreateInfo;

typedef struct {
  UniformHandle target;
  void* data;
} UniformSetInfo;

typedef struct {
  UniformHandle uniform;
  Float4 data;
} UniformBlockItem;

typedef struct {
  UniformHandle uniform;
  TextureHandle texture;
} UniformBlockSampler;

typedef struct {
  const UniformBlockItem* items;
  uint32_t count;
} UniformBlockCreateInfo;

typedef struct {
  VertexLayout layout;
  const Memory memory;
} VertexBufferCreateInfo;

typedef struct {
  const char* name;
} ViewCreateInfo;

typedef struct {
  float depth;
  uint32_t color;
} ViewClearInfo;

typedef struct {
  const Float4x4 view;
  const Float4x4 proj;
} ViewTransformInfo;

typedef struct {
  const Int2 from;
  const Int2 size;
} ViewRectInfo;

typedef struct {
  Float3 pos;
} PosVertex;

typedef struct {
  Float3    pos;
  uint32_t  color;
} PosColorVertex;

typedef struct {
	Float3    pos;
	uint32_t  color;
  Float2    uv;
} PosColorUvVertex;

typedef struct {
	Float3    pos;
  uint8_t indices[4];
} PosIndicesVertex;

typedef struct {
  bool debug;
  bool profile;
} GraphicsInitInfo;

typedef struct {
  const Memory memory;
} ShaderCreateInfo;

//#####################################################################################################################
// Object management
//#####################################################################################################################

KB_RESOURCE_HASHED_FUNC_DECLS (framebuffer   , FramebufferHandle  , FramebufferCreateInfo  )
KB_RESOURCE_HASHED_FUNC_DECLS (index_buffer  , IndexBufferHandle  , IndexBufferCreateInfo  )
KB_RESOURCE_HASHED_FUNC_DECLS (material      , MaterialHandle     , MaterialCreateInfo     )
KB_RESOURCE_HASHED_FUNC_DECLS (mesh          , MeshHandle         , MeshCreateInfo         )
KB_RESOURCE_HASHED_FUNC_DECLS (program       , ProgramHandle      , ProgramCreateInfo      )
KB_RESOURCE_HASHED_FUNC_DECLS (shader        , ShaderHandle       , ShaderCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture       , TextureHandle      , TextureCreateInfo      )
KB_RESOURCE_HASHED_FUNC_DECLS (uniform       , UniformHandle      , UniformCreateInfo      )
KB_RESOURCE_HASHED_FUNC_DECLS (uniform_block , UniformBlockHandle , UniformBlockCreateInfo )
KB_RESOURCE_HASHED_FUNC_DECLS (vertex_buffer , VertexBufferHandle , VertexBufferCreateInfo )
KB_RESOURCE_HASHED_FUNC_DECLS (view          , ViewHandle         , ViewCreateInfo         )

KB_RESOURCE_CORE_FUNC_DECLS   (batch         , BatchHandle        , BatchCreateInfo        )
KB_RESOURCE_CORE_FUNC_DECLS   (encoder       , EncoderHandle      , EncoderCreateInfo      )
KB_RESOURCE_CORE_FUNC_DECLS   (framebuffer   , FramebufferHandle  , FramebufferCreateInfo  )
KB_RESOURCE_CORE_FUNC_DECLS   (gizmo         , GizmoHandle        , GizmoCreateInfo        )
KB_RESOURCE_CORE_FUNC_DECLS   (index_buffer  , IndexBufferHandle  , IndexBufferCreateInfo  )
KB_RESOURCE_CORE_FUNC_DECLS   (material      , MaterialHandle     , MaterialCreateInfo     )
KB_RESOURCE_CORE_FUNC_DECLS   (mesh          , MeshHandle         , MeshCreateInfo         )
KB_RESOURCE_CORE_FUNC_DECLS   (program       , ProgramHandle      , ProgramCreateInfo      )
KB_RESOURCE_CORE_FUNC_DECLS   (shader        , ShaderHandle       , ShaderCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (texture       , TextureHandle      , TextureCreateInfo      )
KB_RESOURCE_CORE_FUNC_DECLS   (uniform       , UniformHandle      , UniformCreateInfo      )
KB_RESOURCE_CORE_FUNC_DECLS   (uniform_block , UniformBlockHandle , UniformBlockCreateInfo )
KB_RESOURCE_CORE_FUNC_DECLS   (vertex_buffer , VertexBufferHandle , VertexBufferCreateInfo )
KB_RESOURCE_CORE_FUNC_DECLS   (view          , ViewHandle         , ViewCreateInfo         )

//#####################################################################################################################
// Static
//#####################################################################################################################

extern VertexLayout layout_pos_vertex;
extern VertexLayout layout_pos_color_vertex;
extern VertexLayout layout_pos_color_uv_vertex;
extern VertexLayout layout_pos_indices_vertex;

//#####################################################################################################################
// API functions
//#####################################################################################################################

KB_API void   kb_batch_set_data               (BatchHandle batch, uint32_t idx, const void* data);

KB_API void   kb_encoder_set_vertex_buffer    (EncoderHandle encoder, VertexBufferHandle handle);
KB_API void   kb_encoder_set_index_buffer     (EncoderHandle encoder, IndexBufferHandle handle);
KB_API void   kb_encoder_set_mesh             (EncoderHandle encoder, MeshHandle handle);
KB_API void   kb_encoder_set_uniform_block    (EncoderHandle encoder, UniformBlockHandle handle);
KB_API void   kb_encoder_set_material         (EncoderHandle encoder, MaterialHandle handle);
KB_API void   kb_encoder_set_transform        (EncoderHandle encoder, const Float4x4 trans);
KB_API void   kb_encoder_set_view             (EncoderHandle encoder, ViewHandle handle);
KB_API void   kb_encoder_set_program          (EncoderHandle encoder, ProgramHandle handle);
KB_API void   kb_encoder_set_uniform          (EncoderHandle encoder, const UniformSetInfo info);
KB_API void   kb_encoder_set_texture          (EncoderHandle encoder, const TextureSetInfo info);
KB_API void   kb_encoder_discard_state        (EncoderHandle encoder);
KB_API void   kb_encoder_render_batch         (EncoderHandle encoder, BatchHandle handle);
KB_API void   kb_encoder_touch_view           (EncoderHandle encoder, ViewHandle view);
KB_API void   kb_encoder_submit               (EncoderHandle encoder, SubmitType type);

KB_API void   kb_gizmo_state_push             (GizmoHandle gizmo);
KB_API void   kb_gizmo_state_pop              (GizmoHandle gizmo);
KB_API void   kb_gizmo_state_push_transform   (GizmoHandle gizmo, const Float4x4 mtx, bool flush);
KB_API void   kb_gizmo_state_pop_transform    (GizmoHandle gizmo, bool flush);
KB_API void   kb_gizmo_state_set_color        (GizmoHandle gizmo, Float4 color);
KB_API void   kb_gizmo_state_set_color_uint   (GizmoHandle gizmo, uint32_t color);
KB_API void   kb_gizmo_state_set_lod          (GizmoHandle gizmo, uint8_t lod);
KB_API void   kb_gizmo_state_set_wireframe    (GizmoHandle gizmo, bool wireframe);
KB_API void   kb_gizmo_shape_close            (GizmoHandle gizmo);
KB_API void   kb_gizmo_shape_move_to          (GizmoHandle gizmo, const Float3 pos);
KB_API void   kb_gizmo_shape_line_to          (GizmoHandle gizmo, const Float3 pos);
KB_API void   kb_gizmo_draw_arc               (GizmoHandle gizmo, Axis axis, const Float3 pos, float radius, float degrees);
KB_API void   kb_gizmo_draw_circle            (GizmoHandle gizmo, const Float3 normal, const Float3 center, float radius, float weight);
KB_API void   kb_gizmo_draw_cone              (GizmoHandle gizmo, const Float3 from, const Float3 to, float radius);
KB_API void   kb_gizmo_draw_cylinder          (GizmoHandle gizmo, const Float3 from, const Float3 to, float radius);
KB_API void   kb_gizmo_draw_axis              (GizmoHandle gizmo, const Float3 pos, float len, float thickness);
KB_API void   kb_gizmo_draw_grid              (GizmoHandle gizmo, Axis axis, const Float3 center, uint32_t size, float step);
KB_API void   kb_gizmo_shape_draw_sphere      (GizmoHandle gizmo, const Sphere sphere);
KB_API void   kb_gizmo_shape_draw_cylinder    (GizmoHandle gizmo, const Cylinder cylinder);
KB_API void   kb_gizmo_shape_draw_disk        (GizmoHandle gizmo, const Disk disk);
KB_API void   kb_gizmo_shape_draw_aabb        (GizmoHandle gizmo, const Aabb aabb);
KB_API void   kb_gizmo_shape_draw_triangle    (GizmoHandle gizmo, const Triangle triangle);
KB_API void   kb_gizmo_shape_draw_ray         (GizmoHandle gizmo, const Ray ray);
KB_API void   kb_gizmo_flush                  (GizmoHandle gizmo);
KB_API void   kb_gizmo_flush_quad             (GizmoHandle gizmo);

KB_API void   kb_view_set_clear               (ViewHandle view, const ViewClearInfo     info);
KB_API void   kb_view_set_transform           (ViewHandle view, const ViewTransformInfo info);
KB_API void   kb_view_set_rect                (ViewHandle view, const ViewRectInfo      info);

KB_API void   kb_graphics_init                (const GraphicsInitInfo info);
KB_API void   kb_graphics_deinit              ();
KB_API void   kb_graphics_frame               ();
KB_API void   kb_graphics_resize              (Int2 size);

KB_API void   kb_debug_overlay_change_mode    ();
KB_API void   kb_debug_overlay_set_mode       (DebugOverlayMode mode);
KB_API void   kb_debug_overlay_add_text       (const char* str);

#ifdef __cplusplus
}
#endif
