#pragma once


#include <kb/math.h>
#include <kb/handle.h>
#include <kb/hash.h>
#include <kb/vertex.h>
#include <kb/rwops.h>

KB_HANDLE(kb_geometry);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_primitive_data {
  uint32_t  first_vertex;
  uint32_t  vertex_count;

  uint32_t  first_index;
  uint32_t  index_count;

  int32_t   material;

  Sphere    sphere;
  Aabb      aabb;
} kb_primitive_data;

typedef struct kb_mesh_data {
  char                name[KB_CONFIG_MAX_NAME_SIZE];
  uint32_t            primitive_count;
  kb_primitive_data*  primitives;
} kb_mesh_data;

typedef struct kb_node_data {
  char                name[KB_CONFIG_MAX_NAME_SIZE];
  XForm               xform;
  int32_t             mesh;
  uint32_t            children_count;
  uint32_t*           children;
} kb_node_data;

typedef struct kb_geometry_data {
  uint32_t            mesh_count;
  uint32_t            node_count;
  uint32_t            material_count;
  uint32_t            index_size;
  uint32_t            index_data_size;
  uint32_t            vertex_data_size;
  kb_vertex_layout    vertex_layout;
  kb_mesh_data*       meshes;
  kb_node_data*       nodes;
  kb_hash*            materials;
  void*               index_data;
  void*               vertex_data;
} kb_geometry_data;

KB_API void kb_geometry_read      (kb_geometry_data* geometry, kb_rwops* rwops);
KB_API void kb_geometry_write     (const kb_geometry_data* geometry, kb_rwops* rwops);
KB_API void kb_geometry_dump_info (const kb_geometry_data* geometry);
KB_API void kb_geometry_deinit    (kb_geometry_data* geometry);

#ifdef __cplusplus
}
#endif
