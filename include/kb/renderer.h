#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <kb/config.h>
#include <kb/hash.h>
#include <kb/resource.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>

#include <kb/geometry.h>
#include <kb/texture.h>

//#####################################################################################################################
// Structs
//#####################################################################################################################

KB_HANDLE(CommandBufferHandle);
KB_HANDLE(IndexBufferHandle);
KB_HANDLE(VertexBufferHandle);
KB_HANDLE(MeshHandle);
KB_HANDLE(ProgramHandle);
KB_HANDLE(UniformSetHandle);
KB_HANDLE(GizmoHandle);

// KB_HANDLE(TextureHandle);
// KB_HANDLE(GeometryHandle);

//#####################################################################################################################
// Enums
//#####################################################################################################################

typedef enum {
  KB_DRAW_SINGLE    = 0,
  KB_CULL_INSTANCED = 1,
} DrawMode;

typedef enum {
  KB_BIND_TYPE_UNSUPPORTED    = 0,
  KB_BIND_TYPE_UNIFORM_BUFFER = 1,
  KB_BIND_TYPE_PUSH_CONSTANT  = 2,
  KB_BIND_TYPE_SAMPLER        = 3,
  KB_BIND_TYPE_STORAGE_BUFFER = 4,
} BindType;

typedef enum {
  KB_CULL_NONE    = 0,
  KB_CULL_BACK    = 1,
  KB_CULL_FRONT   = 2,
  KB_CULL_BOTH    = 3,
} CullMode;

typedef enum {
  KB_TOPOLOGY_TRIANGLE_STRIP  = 0,
  KB_TOPOLOGY_TRIANGLE_LIST   = 1,
  KB_TOPOLOGY_LINE_STRIP      = 2,
  KB_TOPOLOGY_LINE_LIST       = 3,
  KB_TOPOLOGY_POINT_LIST      = 4,
} TopologyType;

//#####################################################################################################################
// Structs
//#####################################################################################################################

// typedef struct {
//   RWops* rwops;
// } GeometryCreateInfo;

typedef struct {
  VertexBufferHandle  vertex_buffer;
  IndexBufferHandle   index_buffer;
  
  uint32_t            material_count;
  UniformSetHandle*   materials;

  uint32_t            mesh_count;
  MeshHandle*         meshes;
} GeometryCreateInfo;

typedef struct {
  int param;
} CommandBufferCreateInfo;

typedef struct {
  RWops*      rwops;
  TextureInfo texture;
  SamplerInfo sampler;
} TextureCreateInfo;

typedef struct {
  RWops*    rwops;
  uint32_t  index_size;
  uint32_t  size;
  bool      host_mapped;
} IndexBufferCreateInfo;

typedef struct {
  RWops*        rwops;
  VertexLayout  layout;
  uint32_t      size;
  bool          host_mapped;
} VertexBufferCreateInfo;

typedef struct {
  VertexBufferHandle  vertex_buffer;
  IndexBufferHandle   index_buffer;

  uint32_t            primitive_count;
  Primitive*          primitives;
} MeshCreateInfo;

typedef struct {
  RWops* vert_code_rwops;
  RWops* frag_code_rwops;
  
  const char* vert_entry = "main";
  const char* frag_entry = "main";

  CullMode      cull        = KB_CULL_BACK;
  TopologyType  topology    = KB_TOPOLOGY_TRIANGLE_LIST;
  DrawMode      mode        = KB_DRAW_SINGLE;
  bool          depth_write = true;
  bool          depth_test  = true;
} ProgramCreateInfo;

typedef struct {
  uint32_t  msaa;
  bool      vsync;
} GraphicsInitInfo;

typedef struct {

} BeginInfo;

typedef struct {
  BindType  type;
  uint32_t  set;
  uint32_t  binding;
  uint32_t  size;
  uint32_t  offset;
} BindSlot;

//#####################################################################################################################
// Object management
//#####################################################################################################################

KB_RESOURCE_HASHED_FUNC_DECLS (index_buffer   , IndexBufferHandle   , IndexBufferCreateInfo   )
KB_RESOURCE_HASHED_FUNC_DECLS (mesh           , MeshHandle          , MeshCreateInfo          )
KB_RESOURCE_HASHED_FUNC_DECLS (program        , ProgramHandle       , ProgramCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , TextureHandle       , TextureCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (vertex_buffer  , VertexBufferHandle  , VertexBufferCreateInfo  )

KB_RESOURCE_CORE_FUNC_DECLS   (geometry       , GeometryHandle      , GeometryCreateInfo      )

KB_RESOURCE_CORE_FUNC_DECLS   (command_buffer , CommandBufferHandle , CommandBufferCreateInfo )
KB_RESOURCE_CORE_FUNC_DECLS   (index_buffer   , IndexBufferHandle   , IndexBufferCreateInfo   )
KB_RESOURCE_CORE_FUNC_DECLS   (mesh           , MeshHandle          , MeshCreateInfo          )
KB_RESOURCE_CORE_FUNC_DECLS   (program        , ProgramHandle       , ProgramCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (texture        , TextureHandle       , TextureCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (vertex_buffer  , VertexBufferHandle  , VertexBufferCreateInfo  )

//#####################################################################################################################
// API functions
//#####################################################################################################################

KB_API CommandBufferHandle  kb_command_buffer_begin               ();
KB_API void                 kb_command_buffer_end                 (CommandBufferHandle command_buffer);

KB_API void*                kb_vertex_buffer_get_mapped           (VertexBufferHandle handle);
KB_API void*                kb_index_buffer_get_mapped            (IndexBufferHandle handle);

KB_API void                 kb_command_buffer_bind_vertex_buffer  (CommandBufferHandle command_buffer, VertexBufferHandle handle);
KB_API void                 kb_command_buffer_bind_index_buffer   (CommandBufferHandle command_buffer, IndexBufferHandle handle);
KB_API void                 kb_command_buffer_set_mesh            (CommandBufferHandle command_buffer, MeshHandle handle);
KB_API void                 kb_command_buffer_bind_program        (CommandBufferHandle command_buffer, ProgramHandle handle);
KB_API void                 kb_command_buffer_set_viewport        (CommandBufferHandle command_buffer, Int2 size, Float2 depth_range);
KB_API void                 kb_command_buffer_set_scissors        (CommandBufferHandle command_buffer, Int2 extent, Int2 offset);
KB_API void                 kb_command_buffer_set_push_constants  (CommandBufferHandle command_buffer, void* data, size_t size);
KB_API void                 kb_command_buffer_submit_mesh         (CommandBufferHandle command_buffer, MeshHandle mesh);
KB_API void                 kb_command_buffer_submit              (CommandBufferHandle handle, uint32_t index_offset, uint32_t vertex_offset, uint32_t index_count);

KB_API void                 kb_command_buffer_push_uniform        (CommandBufferHandle command_buffer, const char* name, const void*, uint32_t size);
KB_API void                 kb_command_buffer_push_texture        (CommandBufferHandle command_buffer, const char* name, TextureHandle texture);

KB_API void                 kb_command_buffer_bind_data           (CommandBufferHandle command_buffer, const BindSlot* slot, const void* data);
KB_API void                 kb_command_buffer_bind_texture        (CommandBufferHandle command_buffer, const BindSlot* slot, TextureHandle texture);

KB_API void                 kb_graphics_init                      (const GraphicsInitInfo info);
KB_API void                 kb_graphics_deinit                    ();
KB_API void                 kb_graphics_frame                     ();
KB_API void                 kb_graphics_frame_tmp_render          ();
KB_API Int2                 kb_graphics_get_extent                ();
KB_API void                 kb_graphics_wait_device_idle          ();

KB_API bool                 kb_program_get_bind_slot              (ProgramHandle program, const char* name, BindSlot* bind_slot);

KB_API VertexBufferHandle   kb_mesh_get_vertex_buffer             (MeshHandle handle);
KB_API IndexBufferHandle    kb_mesh_get_index_buffer              (MeshHandle handle);

KB_API void                 kb_text_overlay_print                 (uint32_t x, uint32_t y, const char* text);
KB_API void                 kb_text_overlay_printf                (uint32_t x, uint32_t y, const char* fmt, ...);

// KB_API TextureHandle        kb_texture_load_png                   (const TextureCreateInfo& info);

KB_API GizmoHandle          kb_gizmo_begin                        (const Float4x4 view, const Float4x4 proj);
KB_API void                 kb_gizmo_end                          (GizmoHandle gizmo);
KB_API void                 kb_gizmo_state_push                   (GizmoHandle gizmo);
KB_API void                 kb_gizmo_state_pop                    (GizmoHandle gizmo);
KB_API void                 kb_gizmo_state_push_transform         (GizmoHandle gizmo, const Float4x4 mtx, bool flush);
KB_API void                 kb_gizmo_state_pop_transform          (GizmoHandle gizmo, bool flush);
KB_API void                 kb_gizmo_state_set_color              (GizmoHandle gizmo, Float4 color);
KB_API void                 kb_gizmo_state_set_lod                (GizmoHandle gizmo, uint8_t lod);
KB_API void                 kb_gizmo_state_set_wireframe          (GizmoHandle gizmo, bool wireframe);
KB_API void                 kb_gizmo_shape_close                  (GizmoHandle gizmo);
KB_API void                 kb_gizmo_shape_move_to                (GizmoHandle gizmo, const Float3 pos);
KB_API void                 kb_gizmo_shape_line_to                (GizmoHandle gizmo, const Float3 pos);
KB_API void                 kb_gizmo_draw_arc                     (GizmoHandle gizmo, Axis axis, const Float3 pos, float radius, float degrees);
KB_API void                 kb_gizmo_draw_circle                  (GizmoHandle gizmo, const Float3 normal, const Float3 center, float radius, float weight);
KB_API void                 kb_gizmo_draw_cone                    (GizmoHandle gizmo, const Float3 from, const Float3 to, float radius);
KB_API void                 kb_gizmo_draw_cylinder                (GizmoHandle gizmo, const Float3 from, const Float3 to, float radius);
KB_API void                 kb_gizmo_draw_axis                    (GizmoHandle gizmo, const Float3 pos, float length);
KB_API void                 kb_gizmo_draw_grid                    (GizmoHandle gizmo, Axis axis, const Float3 center, uint32_t size, float step);
KB_API void                 kb_gizmo_draw_aabb                    (GizmoHandle gizmo, const Aabb aabb);
KB_API void                 kb_gizmo_shape_draw_sphere            (GizmoHandle gizmo, const Sphere sphere);
KB_API void                 kb_gizmo_shape_draw_cylinder          (GizmoHandle gizmo, const Cylinder cylinder);
KB_API void                 kb_gizmo_shape_draw_disk              (GizmoHandle gizmo, const Disk disk);
KB_API void                 kb_gizmo_shape_draw_triangle          (GizmoHandle gizmo, const Triangle triangle);
KB_API void                 kb_gizmo_shape_draw_ray               (GizmoHandle gizmo, const Ray ray);
KB_API void                 kb_gizmo_flush                        (GizmoHandle gizmo);
KB_API void                 kb_gizmo_flush_quad                   (GizmoHandle gizmo);
#ifdef __cplusplus
}
#endif
