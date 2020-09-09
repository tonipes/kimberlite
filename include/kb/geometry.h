#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "math.h"
#include "vertex_layout.h"
#include "rwops.h"
#include "config.h"

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
  char name[KB_CONFIG_MAX_NAME_SIZE];
  uint32_t primitive_count;
  Primitive* primitives;
};

struct Material {
  char name[KB_CONFIG_MAX_NAME_SIZE];
};

struct Node {
  char      name[KB_CONFIG_MAX_NAME_SIZE];
  XForm     xform;
  int32_t   mesh;
  uint32_t  children_count;
  uint32_t* children;
};

struct Geometry {
  uint32_t mesh_count;
  uint32_t node_count;
  uint32_t material_count;
  uint32_t index_size;
  uint32_t index_data_size;
  uint32_t vertex_data_size;

  VertexLayout vertex_layout;

  Mesh*     meshes;
  Node*     nodes;
  Material* materials;

  void*     index_data;
  void*     vertex_data;
};

void kb_geometry_read(Geometry* geometry, RWops* rwops);
void kb_geometry_write(const Geometry* geometry, RWops* rwops);
void kb_geometry_dump_info(const Geometry* geometry);

void kb_geometry_deinit(Geometry* geometry);

#ifdef __cplusplus
}
#endif
