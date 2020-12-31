// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#define KB_TOOL_ONLY

#include <kb/core.h>
#include <kb/time.h>
#include <kb/array.h>
#include <kb/log.h>
#include <kb/vertex.h>
#include <kb/log.h>

#include <kbextra/cliargs.h>
#include <kbextra/geometry.h>

#include "kb/alloc.cpp"
#include "kb/hash.cpp"
#include "kb/log.cpp"
#include "kb/time.cpp"
#include "kb/thread.cpp"
#include "kb/crt.cpp"
#include "kb/texture.cpp"
#include "kb/vertex.cpp"

#include "kbextra/cliargs.cpp"
#include "kbextra/geometry.cpp"

#include <meshoptimizer/meshoptimizer.h>

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

#include "platform/platform_rwops_sdl.cpp"
// #include "platform/platform_rwops_stdio.cpp"

#define EXIT_FAIL     1
#define EXIT_SUCCESS  0

#define POSITION_TYPE   Float4
#define NORMAL_TYPE     Float4
#define TANGENT_TYPE    Float4
#define TEXCOORD_TYPE   Float4
#define COLOR_TYPE      Float4
#define JOINT_TYPE      Float4
#define WEIGHT_TYPE     Float4

const uint16_t POSITION_COMPONENT_COUNT = sizeof(POSITION_TYPE) / sizeof(float);
const uint16_t NORMAL_COMPONENT_COUNT   = sizeof(NORMAL_TYPE)   / sizeof(float);
const uint16_t TANGENT_COMPONENT_COUNT  = sizeof(TANGENT_TYPE)  / sizeof(float);
const uint16_t TEXCOORD_COMPONENT_COUNT = sizeof(TEXCOORD_TYPE) / sizeof(float);
const uint16_t COLOR_COMPONENT_COUNT    = sizeof(COLOR_TYPE)    / sizeof(float);
const uint16_t JOINT_COMPONENT_COUNT    = sizeof(JOINT_TYPE)    / sizeof(float);
const uint16_t WEIGHT_COMPONENT_COUNT   = sizeof(WEIGHT_TYPE)   / sizeof(float);

using IndexType = uint32_t;
const uint32_t MAX_INDEX_COUNT_PER_PRIMITIVE = 4294967295; // 2^32 - 1

// Temporary storage types
struct PrimitiveParseData {
  uint32_t  first_triangle;
  uint32_t  triangle_count;
  int32_t   material;
};

struct MeshParseData {
  char name[KB_CONFIG_MAX_NAME_SIZE];
  
  uint32_t prim_count;
  PrimitiveParseData* prims;
};

struct VertexIndices {
  uint32_t position;
  uint32_t normal;
  uint32_t tangent;
  uint32_t texcoords;
  uint32_t colors;
  uint32_t weights;
  uint32_t joints;
};

struct IndexTriangle {
	VertexIndices vert[3];
};

struct VertexData {
  kb_array positions;
  kb_array normals;
  kb_array tangents;
  kb_array texcoords;
  kb_array colors;
  kb_array joints;
  kb_array weights;
  kb_array triangles;
};

template <typename T>
int32_t index_from(const T* f, const T* e) {
  if (f == nullptr || e == nullptr || e < f) return -1;
  return e - f;
}

void print_help(const char* error = nullptr) {
  if (error != nullptr) printf("%s", error);

  printf(
    "Kimberlite geomc\n"
    "\tUsage: geomc --input <in> --output <out>"
  );
}

XForm get_node_xform(cgltf_node* node) {
  XForm xform;
  xform.position  = { 0, 0, 0 };
  xform.scale     = { 1, 1, 1 };
  xform.rotation  = { 0, 0, 0, 1 };

  if (node->has_matrix) {
    printf("\t\tTODO: decompose matrix\n");
  }

  if (node->has_translation) {
    xform.position = { node->translation[0], node->translation[1], node->translation[2] };
  }

  if (node->has_rotation) {
    xform.rotation = { node->rotation[0], node->rotation[1], node->rotation[2], node->rotation[3] };
  }

  if (node->has_scale) {
    xform.scale = { node->scale[0], node->scale[1], node->scale[2] };
  }

  return xform;
}

int main(int argc, const char* argv[]) {
  int exit_val = EXIT_FAIL;

	cgltf_result    result;
  uint64_t        total_optim_time  = 0;
  cgltf_options   options           = {};
	cgltf_data*     gltf_data         = nullptr;
  uint32_t        input_size        = 0;
  void*           input_data        = nullptr;

  kb_rwops*       rwops_in          = nullptr;
  kb_rwops*       rwops_out         = nullptr;
  const char*     in_filepath       = nullptr;
  const char*     out_filepath      = nullptr;

  MeshParseData*  parse_meshes      = nullptr;
  VertexData      vertex_data       = {};
  
  kb_array_create(&vertex_data.positions, sizeof(POSITION_TYPE),  0);
  kb_array_create(&vertex_data.normals,   sizeof(NORMAL_TYPE),    0);
  kb_array_create(&vertex_data.tangents,  sizeof(TANGENT_TYPE),   0);
  kb_array_create(&vertex_data.texcoords, sizeof(TEXCOORD_TYPE),  0);
  kb_array_create(&vertex_data.colors,    sizeof(COLOR_TYPE),     0);
  kb_array_create(&vertex_data.joints,    sizeof(JOINT_TYPE),     0);
  kb_array_create(&vertex_data.weights,   sizeof(WEIGHT_TYPE),    0);
  kb_array_create(&vertex_data.triangles, sizeof(IndexTriangle),  0);

  // Output geom
  kb_geometry_data geom {};
  geom.index_size = sizeof(IndexType);

  kb_cli_args args {};
  kb_cliargs_init(&args, argc, argv);

  if (kb_cliargs_has(&args, "help")) {
    print_help();
    goto end;
  }

  kb_cliargs_get_str(&args, &in_filepath, "input");
  if (in_filepath == nullptr) {
    print_help("Please specify input file with --input");
    goto end;
  }

  kb_cliargs_get_str(&args, &out_filepath, "output");
  if (out_filepath == nullptr) {
    print_help("Please specify output file with --output");
    goto end;
  }

  rwops_in = kb_rwops_open_file(in_filepath, KB_FILE_MODE_READ);
  if (!rwops_in) {
    print_help("Unable to open input file");
    goto end;
  }

  rwops_out = kb_rwops_open_file(out_filepath, KB_FILE_MODE_WRITE);
  if (!rwops_out) {
    print_help("Unable to open output file");
    goto end;
  }

  // Load
  input_size = kb_rwops_size(rwops_in);
  input_data = KB_DEFAULT_ALLOC(input_size);
  kb_rwops_read(rwops_in, input_data, input_size);

  // Parse GLTF
	result = cgltf_parse(&options, input_data, input_size, &gltf_data);

  if (result != cgltf_result_success) {
    print_help("Unable to parse input file");
    goto end;
  }

  result = cgltf_load_buffers(&options, gltf_data, in_filepath);

  if (result != cgltf_result_success) {
    print_help("Failed to load buffers");
    goto end;
  }

  geom.node_count       = gltf_data->nodes_count;
  geom.mesh_count       = gltf_data->meshes_count;
  geom.material_count   = gltf_data->materials_count;

  geom.nodes            = KB_DEFAULT_ALLOC_TYPE(kb_node_data,     geom.node_count);
  geom.meshes           = KB_DEFAULT_ALLOC_TYPE(kb_mesh_data,     geom.mesh_count);
  geom.materials        = KB_DEFAULT_ALLOC_TYPE(kb_hash,          geom.material_count);

  parse_meshes          = KB_DEFAULT_ALLOC_TYPE(MeshParseData,    geom.mesh_count);

  //#####################################################################################################################
  // Parse
  //#####################################################################################################################
  
  {
    // TODO: Skins
    for (cgltf_size skin_i = 0; skin_i < gltf_data->skins_count; ++skin_i) {
      cgltf_skin* src = &gltf_data->skins[skin_i];
      // kb_log_info("Skin: {}", src->name);
      // if (src->skeleton != NULL) kb_log_info("\tSkeleton: {}", src->skeleton->name);

      // for (uint32_t i = 0; i < src->joints_count; i++) {
        // kb_log_info("\tJoint ({}) {}", i, src->joints[i]->name);
      // }
    }

    // TODO: Animations
    for (cgltf_size anim_i = 0; anim_i < gltf_data->animations_count; ++anim_i) {
      cgltf_animation* src = &gltf_data->animations[anim_i];
      // kb_log_info("Animation: {}", src->name);
      // kb_log_info("\tChannels: {}", src->channels_count);
      // kb_log_info("\tSamplers: {}", src->samplers_count);

      // for (uint32_t i = 0; i < src->samplers_count; i++) {
      //   kb_log_info("\tSampler (%u): interpolation: %u\n", i, src->samplers[i].interpolation);
      // }

      // for (uint32_t i = 0; i < src->channels_count; i++) {
      //   kb_log_info("\tChannel (%u): path: %u, node: (%s)\n", i, src->channels[i].target_path, src->channels[i].target_node->name);
      // }
    }

    // kb_node_datas
    for (cgltf_size node_i = 0; node_i < geom.node_count; ++node_i) {
      geom.nodes[node_i] = {};

      cgltf_node* src = &gltf_data->nodes[node_i];
      kb_node_data*       dst = &geom.nodes[node_i];

      kb_strcpy(dst->name, src->name);
      dst->xform  = get_node_xform(src);
      dst->mesh   = index_from(gltf_data->meshes, src->mesh);
    }

    // Materials
    for (cgltf_size material_i = 0; material_i < geom.material_count; ++material_i) {
      cgltf_material* src = &gltf_data->materials[material_i];
      geom.materials[material_i] = kb_hash_string(src->name);
    }

    // kb_mesh_dataes
    for (cgltf_size mesh_i = 0; mesh_i < geom.mesh_count; ++mesh_i) {
      parse_meshes[mesh_i] = {};
      geom.meshes[mesh_i]  = {};

      cgltf_mesh*     src = &gltf_data->meshes[mesh_i];
      MeshParseData*  dst = &parse_meshes[mesh_i];
      kb_strcpy(dst->name, src->name);

      for (cgltf_size prim_i = 0; prim_i < src->primitives_count; ++prim_i) {
        cgltf_primitive* prim = &src->primitives[prim_i];

        uint32_t prim_first_triangle_index  = kb_array_count(&vertex_data.triangles);

        uint32_t prim_first_position_index  = kb_array_count(&vertex_data.positions);
        uint32_t prim_first_normal_index    = kb_array_count(&vertex_data.normals);
        uint32_t prim_first_tangent_index   = kb_array_count(&vertex_data.tangents);
        uint32_t prim_first_texcoord_index  = kb_array_count(&vertex_data.texcoords);
        uint32_t prim_first_color_index     = kb_array_count(&vertex_data.colors);
        uint32_t prim_first_joints_index    = kb_array_count(&vertex_data.joints);
        uint32_t prim_first_weights_index   = kb_array_count(&vertex_data.weights);

        // Vertex data
        for (cgltf_size attrib_i = 0; attrib_i < prim->attributes_count; ++attrib_i) {
          cgltf_attribute*  attrib          = &prim->attributes[attrib_i];
          cgltf_accessor*   accessor        = attrib->data;
          cgltf_size        accessor_count  = accessor->count;
          cgltf_size        accessor_stride = accessor->stride;
          cgltf_size        accessor_elem_size = cgltf_num_components(accessor->type);
          
          // kb_log_debug("Accessor stride {}", accessor_stride);

          if (attrib->index > 0) {
            printf("ERROR: Only attrib index 0 is currently supported. Sorry!\n");
            break;
          }

          switch (attrib->type) {
            case cgltf_attribute_type_position: {
              uint32_t start = kb_array_count(&vertex_data.positions);
              // uint32_t target_count = accessor_count * POSITION_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.positions, start + accessor_count);

              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.positions, start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_normal: {
              uint32_t start = kb_array_count(&vertex_data.normals);
              // uint32_t target_count = accessor_count * NORMAL_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.normals, start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.normals, start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_tangent: {
              uint32_t start = kb_array_count(&vertex_data.tangents);
              // uint32_t target_count = accessor_count * TANGENT_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.tangents, start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.tangents, start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_texcoord: {
              uint32_t start = kb_array_count(&vertex_data.texcoords);
              // uint32_t target_count = accessor_count * TEXCOORD_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.texcoords, start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.texcoords, start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_color: {
              uint32_t start = kb_array_count(&vertex_data.colors);
              // uint32_t target_count = accessor_count * COLOR_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.colors, start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.colors, start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_joints: {
              uint32_t start = kb_array_count(&vertex_data.joints);
              // uint32_t target_count = accessor_count * JOINT_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.joints, start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.joints, start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_weights: {
              uint32_t start = kb_array_count(&vertex_data.weights);
              // uint32_t target_count = accessor_count * WEIGHT_COMPONENT_COUNT;

              kb_array_resize(&vertex_data.weights, start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) kb_array_get(&vertex_data.weights, start + i), accessor_elem_size);
              }
            } break;

            default: {} break;
          }
        }

        bool has_position = prim_first_position_index < kb_array_count(&vertex_data.positions);      
        bool has_normal   = prim_first_normal_index   < kb_array_count(&vertex_data.normals);
        bool has_tangent  = prim_first_tangent_index  < kb_array_count(&vertex_data.tangents);
        bool has_texcoord = prim_first_texcoord_index < kb_array_count(&vertex_data.texcoords);
        bool has_color    = prim_first_color_index    < kb_array_count(&vertex_data.colors);
        bool has_joints   = prim_first_joints_index   < kb_array_count(&vertex_data.joints);
        bool has_weights  = prim_first_weights_index  < kb_array_count(&vertex_data.weights);

        // Indices
        if (prim->indices) {
          cgltf_size index_count = prim->indices->count;
          for (cgltf_size i = 0; i < index_count; i += 3) { // Tri
            kb_array_resize(&vertex_data.triangles, kb_array_count(&vertex_data.triangles) + 1);            
            IndexTriangle& tri = *(IndexTriangle*) kb_array_back(&vertex_data.triangles);
            
            for (int v = 0; v < 3; ++v) {
              uint32_t vertex_index = cgltf_accessor_read_index(prim->indices, v + i);
          
              tri.vert[v].position  = has_position  ? prim_first_position_index + vertex_index : UINT32_MAX;
              tri.vert[v].normal    = has_normal    ? prim_first_normal_index   + vertex_index : UINT32_MAX;
              tri.vert[v].tangent   = has_tangent   ? prim_first_tangent_index  + vertex_index : UINT32_MAX;
              tri.vert[v].texcoords = has_texcoord  ? prim_first_texcoord_index + vertex_index : UINT32_MAX;
              tri.vert[v].colors    = has_color     ? prim_first_color_index    + vertex_index : UINT32_MAX;
              tri.vert[v].joints    = has_joints    ? prim_first_joints_index   + vertex_index : UINT32_MAX;
              tri.vert[v].weights   = has_weights   ? prim_first_weights_index  + vertex_index : UINT32_MAX;
            }

          }
        }

        // Split to primitives
        uint32_t tri_count = kb_array_count(&vertex_data.triangles) - prim_first_triangle_index;

        uint32_t tri_ind = 0;

        while (tri_ind < tri_count) {
          uint32_t tris_remaining = tri_count - tri_ind;
          uint32_t prim_tri_count = min(tris_remaining, MAX_INDEX_COUNT_PER_PRIMITIVE / 3);

          uint32_t idx = dst->prim_count++;

          dst->prims = KB_DEFAULT_REALLOC_TYPE(PrimitiveParseData, dst->prims, dst->prim_count);
          PrimitiveParseData* prim_out = &dst->prims[idx];
          
          prim_out->first_triangle = prim_first_triangle_index + tri_ind;
          prim_out->triangle_count = prim_tri_count;
          prim_out->material       = index_from(gltf_data->materials, prim->material);

          tri_ind += prim_tri_count;
        }
      }
    }
  }

  //#####################################################################################################################
  // Process & Optimize
  //#####################################################################################################################
  
  {
    bool has_position = false;
    bool has_normal   = false;
    bool has_tangent  = false;
    bool has_texcoord = false;
    bool has_color    = false;
    bool has_joints   = false;
    bool has_weights  = false;

    for (uint64_t tri_i = 0; tri_i < kb_array_count(&vertex_data.triangles); ++tri_i) {
      IndexTriangle& tri = *(IndexTriangle*) kb_array_get(&vertex_data.triangles, tri_i);

      for (uint32_t i = 0; i < 3; ++i) {
        has_position  |= tri.vert[i].position   != UINT32_MAX;
        has_normal    |= tri.vert[i].normal     != UINT32_MAX;
        has_tangent   |= tri.vert[i].tangent    != UINT32_MAX;
        has_texcoord  |= tri.vert[i].texcoords  != UINT32_MAX;
        has_color     |= tri.vert[i].colors     != UINT32_MAX;
        has_weights   |= tri.vert[i].weights    != UINT32_MAX;
        has_joints    |= tri.vert[i].joints     != UINT32_MAX;
      }
    }
    
    int32_t attrib_position  = -1;
    int32_t attrib_normal    = -1;
    int32_t attrib_tangent   = -1;
    int32_t attrib_texcoord  = -1;
    int32_t attrib_color     = -1;
    int32_t attrib_joints    = -1;
    int32_t attrib_weights   = -1;

    kb_vertex_layout_begin(&geom.vertex_layout);
      if (has_position) attrib_position = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, POSITION_COMPONENT_COUNT);
      if (has_normal)   attrib_normal   = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, NORMAL_COMPONENT_COUNT);
      if (has_tangent)  attrib_tangent  = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, TANGENT_COMPONENT_COUNT);
      if (has_texcoord) attrib_texcoord = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, TEXCOORD_COMPONENT_COUNT);
      if (has_color)    attrib_color    = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, COLOR_COMPONENT_COUNT);
      if (has_weights)  attrib_weights  = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, WEIGHT_COMPONENT_COUNT);
      if (has_joints)   attrib_joints   = kb_vertex_layout_add(&geom.vertex_layout, KB_ATTRIB_FLOAT, JOINT_COMPONENT_COUNT);
    kb_vertex_layout_end(&geom.vertex_layout);

    uint32_t vertex_stride = kb_vertex_layout_stride(&geom.vertex_layout);

    uint64_t max_vertex_count = kb_array_count(&vertex_data.triangles) * 3;
    uint64_t max_index_count  = kb_array_count(&vertex_data.triangles) * 3;

    uint8_t*  geom_vert_data  = (uint8_t*)    KB_DEFAULT_ALLOC(max_vertex_count * vertex_stride);
    IndexType* geom_ind_data  = (IndexType*)  KB_DEFAULT_ALLOC(sizeof(IndexType) * max_index_count);
    
    uint64_t vertex_count = 0;
    uint64_t index_count  = 0;
    
    for (uint32_t i = 0; i < geom.mesh_count; ++i) {
      MeshParseData* mesh = &parse_meshes[i];

      // Init output mesh
      kb_mesh_data* mesh_out            = &geom.meshes[i];
      mesh_out->primitive_count = mesh->prim_count;
      mesh_out->primitives      = KB_DEFAULT_ALLOC_TYPE(kb_primitive_data, mesh_out->primitive_count);
      
      kb_strcpy(mesh_out->name, mesh->name);

      for (uint32_t i = 0; i < mesh->prim_count; i++) {
        PrimitiveParseData* prim = &mesh->prims[i];
    
        uint64_t    prim_vertex_count = prim->triangle_count * 3;
        uint64_t    prim_index_count  = prim->triangle_count * 3;
        
        uint8_t*    prim_vert_data    = KB_DEFAULT_ALLOC_TYPE(uint8_t, prim_index_count * vertex_stride);
        IndexType*  prim_ind_data     = KB_DEFAULT_ALLOC_TYPE(IndexType, prim_index_count);
        
        uint32_t    prim_vert         = 0;
        uint32_t    prim_index        = 0;

        for (uint32_t tri_i = prim->first_triangle; tri_i < prim->first_triangle + prim->triangle_count; ++tri_i) {
          IndexTriangle& tri = *(IndexTriangle*) kb_array_get(&vertex_data.triangles, tri_i);

          for (uint32_t v = 0; v < 3; ++v) {
            const VertexIndices& vert = tri.vert[v];

            if (has_position) {
              uint32_t idx = vert.position == UINT32_MAX ? 0 : vert.position;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_position);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_position);
              
              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.positions, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }

            if (has_normal) {
              uint32_t idx = vert.normal == UINT32_MAX ? 0 : vert.normal;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_normal);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_normal);

              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.normals, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_tangent) {
              uint32_t idx = vert.tangent == UINT32_MAX ? 0 : vert.tangent;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_tangent);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_tangent);

              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.tangents, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }

            if (has_texcoord) {
              uint32_t idx = vert.texcoords == UINT32_MAX ? 0 : vert.texcoords;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_texcoord);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_texcoord);

              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.texcoords, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_color) {
              uint32_t idx = vert.colors == UINT32_MAX ? 0 : vert.colors;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_color);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_color);

              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.colors, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_weights) {
              uint32_t idx = vert.weights == UINT32_MAX ? 0 : vert.weights;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_weights);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_weights);

              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.weights, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_joints) {
              uint32_t idx = vert.joints == UINT32_MAX ? 0 : vert.joints;
              uint32_t attrib_size   = kb_vertex_layout_size(&geom.vertex_layout, attrib_joints);
              uint32_t attrib_offset = kb_vertex_layout_offset(&geom.vertex_layout, attrib_joints);

              kb_memcpy_with_stride(prim_vert_data, kb_array_get(&vertex_data.joints, idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }

            prim_ind_data[prim_index++] = prim_vert;
            prim_vert++;
          }
        }

        // Optimize primitive
        uint64_t opt_start_time = kb_time_get_raw();

        uint32_t* remap = KB_DEFAULT_ALLOC_TYPE(uint32_t, prim_vertex_count * 3);

        uint32_t opt_vertex_count = meshopt_generateVertexRemap(remap, prim_ind_data, prim_index_count, prim_vert_data, prim_vertex_count, vertex_stride);

	      uint8_t*  opt_vert_data   = KB_DEFAULT_ALLOC_TYPE(uint8_t, opt_vertex_count * vertex_stride);
	      IndexType* opt_ind_data   = KB_DEFAULT_ALLOC_TYPE(IndexType, prim_index_count);

        meshopt_remapIndexBuffer      (opt_ind_data,  prim_ind_data,  prim_index_count,   remap);
        meshopt_remapVertexBuffer     (opt_vert_data, prim_vert_data, prim_vertex_count,  vertex_stride,            remap);
        meshopt_optimizeVertexCache   (opt_ind_data,  opt_ind_data,   prim_index_count,   prim_vertex_count);
        meshopt_optimizeOverdraw      (opt_ind_data,  opt_ind_data,   prim_index_count,   (float*) opt_vert_data, prim_vertex_count,  vertex_stride, 1.05f);
        meshopt_optimizeVertexFetch   (opt_vert_data, opt_ind_data,   prim_index_count,   opt_vert_data,          opt_vertex_count,   vertex_stride);

        total_optim_time += kb_time_get_raw() - opt_start_time;

        mesh_out->primitives[i] = {};
        mesh_out->primitives[i].first_vertex  = vertex_count;
        mesh_out->primitives[i].first_index   = index_count;
        mesh_out->primitives[i].vertex_count  = opt_vertex_count;
        mesh_out->primitives[i].index_count   = prim_index_count;
        mesh_out->primitives[i].material      = prim->material;
  

        // Copy data to main buffers
        kb_memcpy(geom_ind_data  + index_count                    , opt_ind_data  , prim_index_count * sizeof(IndexType));
        kb_memcpy(geom_vert_data + vertex_count * vertex_stride   , opt_vert_data , opt_vertex_count * vertex_stride);

        vertex_count  += opt_vertex_count;
        index_count   += prim_index_count;
        
        KB_DEFAULT_FREE(opt_vert_data);
        KB_DEFAULT_FREE(opt_ind_data);
        KB_DEFAULT_FREE(remap);

        KB_DEFAULT_FREE(prim_vert_data);
        KB_DEFAULT_FREE(prim_ind_data);
      }
    }

    // Copy data to geom
    geom.vertex_data_size = vertex_count * vertex_stride;
    geom.index_data_size  = index_count * sizeof(IndexType);

    geom.vertex_data  = KB_DEFAULT_ALLOC(geom.vertex_data_size);
    geom.index_data   = KB_DEFAULT_ALLOC(geom.index_data_size);

    kb_memset(geom.vertex_data, '\0', geom.vertex_data_size);
    kb_memset(geom.index_data,  '\0', geom.index_data_size);

    kb_memcpy(geom.vertex_data, geom_vert_data, geom.vertex_data_size);
    kb_memcpy(geom.index_data,  geom_ind_data, geom.index_data_size);

    KB_DEFAULT_FREE(geom_vert_data);
    KB_DEFAULT_FREE(geom_ind_data);
  }

  kb_log_info("All done! Total optimization time: {}", double(total_optim_time) / double(kb_time_get_frequency()));

  //#####################################################################################################################
  // Export
  //#####################################################################################################################
  
  kb_vertex_layout_dump(&geom.vertex_layout);
  
  // kb_geometry_data_dump_info(&geom);

  kb_geometry_data_write(&geom, rwops_out);
  
  exit_val = EXIT_SUCCESS;

end:
  kb_rwops_close(rwops_in);
  kb_rwops_close(rwops_out);
  
  kb_geometry_data_destroy(&geom);

  KB_DEFAULT_FREE(input_data);

  kb_array_destroy(&vertex_data.positions);
  kb_array_destroy(&vertex_data.normals);
  kb_array_destroy(&vertex_data.tangents);
  kb_array_destroy(&vertex_data.texcoords);
  kb_array_destroy(&vertex_data.colors);
  kb_array_destroy(&vertex_data.joints);
  kb_array_destroy(&vertex_data.weights);
  kb_array_destroy(&vertex_data.triangles);

  return exit_val;
}
