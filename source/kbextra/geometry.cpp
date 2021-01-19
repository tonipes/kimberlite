// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/geometry.h>

#include <kb/foundation.h>
#include <kb/graphics.h>

void kb_geometry_data_dump_info(const kb_geometry_data* geom) {
  KB_ASSERT_NOT_NULL(geom);

  kb_log_debug("Geometry:");
  kb_log_debug("\tNodes: ({}):", geom->node_count);
  
  for (uint32_t i = 0; i < geom->node_count; i++) {
    kb_log_debug("\t\tNode: {} ({}), mesh: {}", i, geom->nodes[i].name, geom->nodes[i].mesh);
  }

  kb_log_debug("\tMeshes: ({}):", geom->mesh_count);
  
  for (uint32_t i = 0; i < geom->mesh_count; i++) {
    kb_log_debug("\t\tMesh {} ({}):", i, geom->meshes[i].name);
    for (uint32_t j = 0; j < geom->meshes[i].primitive_count; j++) {
      kb_log_debug("\t\t\tPrim: index range: ({} - {} ({} indices)), vertex range: ({} - {} ({} verts)), material: {} ", 
        geom->meshes[i].primitives[j].first_index, geom->meshes[i].primitives[j].first_index + geom->meshes[i].primitives[j].index_count, geom->meshes[i].primitives[j].index_count,
        geom->meshes[i].primitives[j].first_vertex, geom->meshes[i].primitives[j].first_vertex + geom->meshes[i].primitives[j].vertex_count, geom->meshes[i].primitives[j].vertex_count,
        geom->meshes[i].primitives[j].material
      );
    }
  }

  kb_log_debug("\tMaterials: ({})", geom->material_count);
  kb_log_debug("\tIndices: ({})", geom->index_data_size / geom->index_size);
  kb_log_debug("\tIndex data: {} bytes", geom->index_data_size);
  kb_log_debug("\tVertex data: {} bytes", geom->vertex_data_size);
}

void kb_geometry_data_read(kb_geometry_data* geom, kb_stream* rwops) {
  KB_ASSERT_NOT_NULL(geom);
  KB_ASSERT_NOT_NULL(rwops);

  if (!kb_stream_check_magic(rwops, KB_CONFIG_FILE_MAGIC_GEOM)) {
    kb_log_debug("Did not find correct magic number!");
    return;
  }

  kb_read(rwops, geom->mesh_count);
  kb_read(rwops, geom->node_count);
  kb_read(rwops, geom->material_count);
  kb_read(rwops, geom->index_size);
  kb_read(rwops, geom->index_data_size);
  kb_read(rwops, geom->vertex_data_size);
  
  // Meshes
  geom->meshes = KB_DEFAULT_ALLOC_TYPE(kb_mesh_data, geom->mesh_count);
  for (uint32_t i = 0; i < geom->mesh_count; i++) {
    kb_read(rwops, geom->meshes[i].primitive_count);
    kb_read(rwops, geom->meshes[i].name);

    geom->meshes[i].primitives = KB_DEFAULT_ALLOC_TYPE(kb_primitive_data, geom->meshes[i].primitive_count);

    for (uint32_t j = 0; j < geom->meshes[i].primitive_count; j++) {
      kb_read(rwops, geom->meshes[i].primitives[j]);
    }
  }

  // Nodes
  geom->nodes = KB_DEFAULT_ALLOC_TYPE(kb_node_data, geom->node_count);
  for (uint32_t i = 0; i < geom->node_count; i++) {
    kb_read(rwops, geom->nodes[i].name);
    kb_read(rwops, geom->nodes[i].xform);
    kb_read(rwops, geom->nodes[i].mesh);
    kb_read(rwops, geom->nodes[i].children_count);

    geom->nodes[i].children = KB_DEFAULT_ALLOC_TYPE(uint32_t, geom->nodes[i].children_count);
    for (uint32_t j = 0; j < geom->nodes[i].children_count; j++) {
      kb_read(rwops, geom->nodes[i].children[j]);
    }
  }

  // Materials
  geom->materials = KB_DEFAULT_ALLOC_TYPE(kb_hash, geom->material_count);
  for (uint32_t i = 0; i < geom->material_count; i++) {
    kb_read(rwops, geom->materials[i]);
  }

  // Vertex data
  geom->vertex_data = KB_DEFAULT_ALLOC(geom->vertex_data_size);
  geom->index_data  = KB_DEFAULT_ALLOC(geom->index_data_size);

  kb_stream_read(rwops, geom->vertex_data,  1, geom->vertex_data_size);
  kb_stream_read(rwops, geom->index_data,   1, geom->index_data_size);
}

void kb_geometry_data_write(const kb_geometry_data* geom, kb_stream* rwops) {
  KB_ASSERT_NOT_NULL(geom);
  KB_ASSERT_NOT_NULL(rwops);

  kb_write(rwops, KB_CONFIG_FILE_MAGIC_GEOM);

  kb_write(rwops, geom->mesh_count);
  kb_write(rwops, geom->node_count);
  kb_write(rwops, geom->material_count);
  kb_write(rwops, geom->index_size);
  kb_write(rwops, geom->index_data_size);
  kb_write(rwops, geom->vertex_data_size);

  // Meshes
  for (uint32_t i = 0; i < geom->mesh_count; i++) {
    kb_write(rwops, geom->meshes[i].primitive_count);
    kb_write(rwops, geom->meshes[i].name);

    for (uint32_t j = 0; j < geom->meshes[i].primitive_count; j++) {
      kb_write(rwops, geom->meshes[i].primitives[j]);
    }
  }

  // Nodes
  for (uint32_t i = 0; i < geom->node_count; i++) {
    kb_write(rwops, geom->nodes[i].name);
    kb_write(rwops, geom->nodes[i].xform);
    kb_write(rwops, geom->nodes[i].mesh);
    kb_write(rwops, geom->nodes[i].children_count);

    for (uint32_t j = 0; j < geom->nodes[i].children_count; j++) {
      kb_write(rwops, geom->nodes[i].children[j]);
    }
  }
    
  // Materials
  for (uint32_t i = 0; i < geom->material_count; i++) {
    kb_write(rwops, geom->materials[i]);
  }

  // Vertex data
  kb_stream_write(rwops, geom->vertex_data,  1, geom->vertex_data_size);
  kb_stream_write(rwops, geom->index_data,   1, geom->index_data_size);
}

void kb_geometry_data_destroy(kb_geometry_data* geom) {
  for (uint32_t i = 0; i < geom->mesh_count; i++) {
    KB_DEFAULT_FREE(geom->meshes[i].primitives);
  }

  for (uint32_t i = 0; i < geom->node_count; i++) {
    KB_DEFAULT_FREE(geom->nodes[i].children);
  }

  KB_DEFAULT_FREE(geom->materials);
  KB_DEFAULT_FREE(geom->meshes);
  KB_DEFAULT_FREE(geom->nodes);
  KB_DEFAULT_FREE(geom->vertex_data);
  KB_DEFAULT_FREE(geom->index_data);
}

#ifndef KB_TOOL_ONLY

struct kb_primitive_ref {
  uint32_t            first_vertex;
  uint32_t            vertex_count;
  uint32_t            first_index;
  uint32_t            index_count;
  kb_material         material;
};

struct kb_mesh_ref {
  kb_buffer           vertex_buffer;
  kb_buffer           index_buffer;
  kb_primitive_ref*   primitives;
  uint32_t            primitive_count;
};

struct kb_geometry_ref {
  kb_buffer           vertex_buffer;
  kb_buffer           index_buffer;
  kb_material*        materials;
  kb_mesh*            meshes;
  uint32_t            material_count;
  uint32_t            mesh_count;
};

KB_RESOURCE_STORAGE_DEF     (mesh, kb_mesh, kb_mesh_ref, KB_CONFIG_MAX_MESHES);
KB_RESOURCE_ALLOC_FUNC_DEF  (mesh, kb_mesh, kb_mesh_create_info, KB_CONFIG_MAX_MESHES);
KB_RESOURCE_DATA_HASHED_DEF (mesh, kb_mesh);

KB_RESOURCE_STORAGE_DEF     (geometry, kb_geometry, kb_geometry_ref, KB_CONFIG_MAX_GEOMS);
KB_RESOURCE_ALLOC_FUNC_DEF  (geometry, kb_geometry, kb_geometry_create_info, KB_CONFIG_MAX_GEOMS);
KB_RESOURCE_DATA_HASHED_DEF (geometry, kb_geometry);

void kb_mesh_construct(kb_mesh handle, const kb_mesh_create_info info) {
  KB_ASSERT_VALID(handle);

  mesh_ref(handle)->vertex_buffer  = info.vertex_buffer;
  mesh_ref(handle)->index_buffer   = info.index_buffer;

  mesh_ref(handle)->primitive_count  = info.primitive_count;
  mesh_ref(handle)->primitives       = KB_DEFAULT_ALLOC_TYPE(kb_primitive_ref, info.primitive_count);

  for (uint32_t i = 0; i < info.primitive_count; ++i) {
    mesh_ref(handle)->primitives[i].first_index  = info.primitives[i].first_index;
    mesh_ref(handle)->primitives[i].first_vertex = info.primitives[i].first_vertex;
    mesh_ref(handle)->primitives[i].index_count  = info.primitives[i].index_count;
    mesh_ref(handle)->primitives[i].vertex_count = info.primitives[i].vertex_count;
    
    mesh_ref(handle)->primitives[i].material = { };
    if (info.material_count > info.primitives[i].material) {
      mesh_ref(handle)->primitives[i].material = info.materials[info.primitives[i].material];
    }
  }

  kb_mesh_set_initialized(handle, true);
}

void kb_mesh_destruct(kb_mesh handle) {
  KB_ASSERT_VALID(handle);

  KB_DEFAULT_FREE(mesh_ref(handle)->primitives);
  kb_mesh_set_initialized(handle, false);
}

void kb_geometry_construct(kb_geometry handle, const kb_geometry_create_info info) {
  KB_ASSERT_VALID(handle);
  
  kb_geometry_data geom {};
  
  kb_geometry_data_read(&geom, info.data);
  
  kb_geometry_data_dump_info(&geom);
  
  // Index buffer
  kb_stream* ib_rwops = kb_stream_open_mem(geom.index_data, geom.index_data_size);
  geometry_ref(handle)->index_buffer = kb_buffer_create({
    .rwops        = ib_rwops,
    .size         = geom.index_data_size,
    .usage        = KB_BUFFER_USAGE_INDEX_BUFFER,
  });
  kb_stream_close(ib_rwops);
  
  // Vertex_buffer
  kb_stream* vb_rwops = kb_stream_open_mem(geom.vertex_data, geom.vertex_data_size);
  geometry_ref(handle)->vertex_buffer = kb_buffer_create({
    .rwops        = vb_rwops,
    .size         = geom.vertex_data_size,
    .usage        = KB_BUFFER_USAGE_VERTEX_BUFFER,
  });
  kb_stream_close(vb_rwops);

  geometry_ref(handle)->material_count = geom.material_count;
  geometry_ref(handle)->materials      = KB_DEFAULT_ALLOC_TYPE(kb_material, geom.material_count);

  for (uint32_t i = 0; i < geom.material_count; i++) {
    geometry_ref(handle)->materials[i] = kb_material_get(geom.materials[i]);
  }
  
  geometry_ref(handle)->mesh_count     = geom.mesh_count;
  geometry_ref(handle)->meshes         = KB_DEFAULT_ALLOC_TYPE(kb_mesh, geom.mesh_count);
  
  for (uint32_t i = 0; i < geom.mesh_count; i++) {
    geometry_ref(handle)->meshes[i] = kb_mesh_create({
      .index_buffer     = geometry_ref(handle)->index_buffer,
      .vertex_buffer    = geometry_ref(handle)->vertex_buffer,
      .primitive_count  = geom.meshes[i].primitive_count,
      .primitives       = geom.meshes[i].primitives,
      .material_count   = geom.material_count,
      .materials        = geometry_ref(handle)->materials,
    });
        
    kb_mesh_mark(geometry_ref(handle)->meshes[i], kb_hash_string(geom.meshes[i].name));
  }

  kb_geometry_set_initialized(handle, true);
}

void kb_geometry_destruct(kb_geometry handle) {
  KB_ASSERT_VALID(handle);
  
  kb_buffer_destroy(geometry_ref(handle)->index_buffer);
  kb_buffer_destroy(geometry_ref(handle)->vertex_buffer);

  for (uint32_t i = 0; i < geometry_ref(handle)->mesh_count; i++) {
    kb_mesh_destroy(geometry_ref(handle)->meshes[i]);
  }

  KB_DEFAULT_FREE(geometry_ref(handle)->meshes);
  KB_DEFAULT_FREE(geometry_ref(handle)->materials);
  
  kb_geometry_set_initialized(handle, false);
}

KB_API void kb_encoder_submit_primitive_draw(kb_encoder encoder, kb_mesh mesh, uint32_t prim_index, uint32_t instance_count) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(mesh);

  KB_ASSERT(mesh_ref(mesh)->primitive_count > prim_index, "Primitive index too large");
  
  kb_encoder_bind_vertex_buffer(encoder, 0, mesh_ref(mesh)->vertex_buffer, 0);
  kb_encoder_bind_index_buffer(encoder, mesh_ref(mesh)->index_buffer, 0, KB_INDEX_TYPE_32);
//  kb_encoder_bind_index_buffer(encoder, mesh_ref(mesh)->index_buffer, 0, KB_INDEX_TYPE_16);

  kb_primitive_ref& primitive = mesh_ref(mesh)->primitives[prim_index];

  kb_encoder_submit(encoder, 
    primitive.first_index,
    primitive.first_vertex,
    primitive.index_count,
    instance_count
  );
  
}

KB_API void kb_encoder_submit_mesh(kb_encoder encoder, kb_mesh mesh, uint32_t instance_count, bool bind_material) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(mesh);
  
  for (uint32_t i = 0; i < mesh_ref(mesh)->primitive_count; ++i) {
    kb_encoder_push(encoder);

    kb_material material = mesh_ref(mesh)->primitives[i].material;
    
    if (bind_material && kb_is_valid(material) && kb_material_is_initialized(material)) {
      kb_encoder_bind_material(encoder, material);
    }
    
    kb_encoder_submit_primitive_draw(encoder, mesh, i, instance_count);
    kb_encoder_pop(encoder);
  }  
  
}

#endif
