#pragma once

#include <kb/types.h>
#include <kb/math.h>
#include <kb/handle.h>
#include <kb/hash.h>
#include <kb/vertex_layout.h>
#include <kb/rwops.h>
#include <kb/config.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(GeometryHandle);

struct Primitive {
  uint32_t  first_vertex;
  uint32_t  vertex_count;

  uint32_t  first_index;
  uint32_t  index_count;

  int32_t   material;

  Sphere    sphere;
  Aabb      aabb;
};

struct Mesh {
  char          name[KB_CONFIG_MAX_NAME_SIZE];
  uint32_t      primitive_count;
  Primitive*    primitives;
};

struct Node {
  char          name[KB_CONFIG_MAX_NAME_SIZE];
  XForm         xform;
  int32_t       mesh;
  uint32_t      children_count;
  uint32_t*     children;
};

struct Geometry {
  uint32_t      mesh_count;
  uint32_t      node_count;
  uint32_t      material_count;
  uint32_t      index_size;
  uint32_t      index_data_size;
  uint32_t      vertex_data_size;

  VertexLayout  vertex_layout;

  Mesh*         meshes;
  Node*         nodes;
  Hash*         materials;

  void*         index_data;
  void*         vertex_data;
};

KB_API void kb_geometry_read(Geometry* geometry, RWops* rwops);
KB_API void kb_geometry_write(const Geometry* geometry, RWops* rwops);
KB_API void kb_geometry_dump_info(const Geometry* geometry);

KB_API void kb_geometry_deinit(Geometry* geometry);

#ifdef __cplusplus
}
#endif
