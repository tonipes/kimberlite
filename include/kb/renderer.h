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

//#####################################################################################################################
// Structs
//#####################################################################################################################

KB_HANDLE(CommandBufferHandle);
KB_HANDLE(IndexBufferHandle);
KB_HANDLE(VertexBufferHandle);
KB_HANDLE(MeshHandle);
KB_HANDLE(ProgramHandle);
KB_HANDLE(TextureHandle);

//#####################################################################################################################
// Enums
//#####################################################################################################################

typedef enum {
  KB_DRAW_SINGLE    = 0,
  KB_CULL_INSTANCED = 1,
} DrawMode;

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

typedef struct {
  int param;
} CommandBufferCreateInfo;

typedef struct {
  RWops* rwops;
} IndexBufferCreateInfo;

typedef struct {
  RWops* rwops;
} TextureCreateInfo;

typedef struct {
  RWops* rwops;
} VertexBufferCreateInfo;

typedef struct {
  uint32_t first_vertex;
  uint32_t vertex_count;
  VertexBufferHandle vertex_buffer;

  uint32_t first_index;
  uint32_t index_count;
  IndexBufferHandle index_buffer;
} MeshCreateInfo;

typedef struct {
  RWops* vert_code_rwops;
  RWops* frag_code_rwops;
  
  const char* vert_entry = "main";
  const char* frag_entry = "main";

  CullMode      cull      = KB_CULL_BACK;
  TopologyType  topology  = KB_TOPOLOGY_TRIANGLE_LIST;
  DrawMode      mode      = KB_DRAW_SINGLE;
} ProgramCreateInfo;

typedef struct {
  bool debug;
  bool profile;
} GraphicsInitInfo;

typedef struct {

} BeginInfo;

//#####################################################################################################################
// Object management
//#####################################################################################################################

KB_RESOURCE_HASHED_FUNC_DECLS (index_buffer   , IndexBufferHandle   , IndexBufferCreateInfo   )
KB_RESOURCE_HASHED_FUNC_DECLS (mesh           , MeshHandle          , MeshCreateInfo          )
KB_RESOURCE_HASHED_FUNC_DECLS (program        , ProgramHandle       , ProgramCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , TextureHandle       , TextureCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (vertex_buffer  , VertexBufferHandle  , VertexBufferCreateInfo  )

KB_RESOURCE_CORE_FUNC_DECLS   (command_buffer , CommandBufferHandle , CommandBufferCreateInfo )
KB_RESOURCE_CORE_FUNC_DECLS   (index_buffer   , IndexBufferHandle   , IndexBufferCreateInfo   )
KB_RESOURCE_CORE_FUNC_DECLS   (mesh           , MeshHandle          , MeshCreateInfo          )
KB_RESOURCE_CORE_FUNC_DECLS   (program        , ProgramHandle       , ProgramCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (texture        , TextureHandle       , TextureCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (vertex_buffer  , VertexBufferHandle  , VertexBufferCreateInfo  )

//#####################################################################################################################
// API functions
//#####################################################################################################################

KB_API CommandBufferHandle  kb_command_buffer_begin             ();
KB_API void                 kb_command_buffer_end               (CommandBufferHandle command_buffer);

KB_API void                 kb_command_buffer_bind_vertex_buffer (CommandBufferHandle command_buffer, VertexBufferHandle handle);
KB_API void                 kb_command_buffer_bind_index_buffer  (CommandBufferHandle command_buffer, IndexBufferHandle handle);
KB_API void                 kb_command_buffer_set_mesh          (CommandBufferHandle command_buffer, MeshHandle handle);
KB_API void                 kb_command_buffer_bind_program       (CommandBufferHandle command_buffer, ProgramHandle handle);
KB_API void                 kb_command_buffer_set_viewport      (CommandBufferHandle command_buffer, Float2 size, Float2 depth_range);
KB_API void                 kb_command_buffer_set_scissors      (CommandBufferHandle command_buffer, Int2 extent, Int2 offset);
KB_API void                 kb_command_buffer_set_push_constants(CommandBufferHandle command_buffer, void* data, size_t size);
KB_API void                 kb_command_buffer_submit            (CommandBufferHandle command_buffer);

KB_API void                 kb_command_buffer_push_uniform      (CommandBufferHandle command_buffer, const char* name, const void*, uint32_t size);
KB_API void                 kb_command_buffer_push_texture      (CommandBufferHandle command_buffer, const char* name, TextureHandle texture);

KB_API void                 kb_graphics_init                    (const GraphicsInitInfo info);
KB_API void                 kb_graphics_deinit                  ();
KB_API void                 kb_graphics_frame                   ();
KB_API void                 kb_graphics_frame_tmp_render        ();
KB_API Int2                 kb_graphics_get_extent              ();
KB_API void                 kb_graphics_wait_device_idle        ();

#ifdef __cplusplus
}
#endif
