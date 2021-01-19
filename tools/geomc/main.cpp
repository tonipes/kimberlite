// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================


#define KB_TOOL_ONLY


#include <kb/foundation/core.h>
#include <kb/foundation/time.h>
#include <kb/foundation/array.h>

#include <kb/log.h>

#include <kbextra/cliargs.h>
#include <kbextra/geometry.h>
#include <kbextra/vertex.h>

#include "kb/foundation.cpp"
#include "kb/log.cpp"

#include "kbextra/texture.cpp"
#include "kbextra/vertex.cpp"
#include "kbextra/cliargs.cpp"
#include "kbextra/geometry.cpp"

#include <meshoptimizer/meshoptimizer.h>
#include <meshoptimizer/allocator.cpp>
#include <meshoptimizer/clusterizer.cpp>
#include <meshoptimizer/indexcodec.cpp>
#include <meshoptimizer/indexgenerator.cpp>
#include <meshoptimizer/overdrawanalyzer.cpp>
#include <meshoptimizer/overdrawoptimizer.cpp>
#include <meshoptimizer/simplifier.cpp>
#include <meshoptimizer/spatialorder.cpp>
#include <meshoptimizer/stripifier.cpp>
#include <meshoptimizer/vcacheanalyzer.cpp>
#include <meshoptimizer/vcacheoptimizer.cpp>
#include <meshoptimizer/vertexcodec.cpp>
#include <meshoptimizer/vertexfilter.cpp>
#include <meshoptimizer/vfetchanalyzer.cpp>
#include <meshoptimizer/vfetchoptimizer.cpp>

#define CGLTF_IMPLEMENTATION
#include <cgltf/cgltf.h>

#include <platform/platform_rwops_stdio.cpp>

#define EXIT_FAIL     1
#define EXIT_SUCCESS  0

#define POSITION_TYPE   kb::float4
#define NORMAL_TYPE     kb::float4
#define TANGENT_TYPE    kb::float4
#define TEXCOORD_TYPE   kb::float4
#define COLOR_TYPE      kb::float4
#define JOINT_TYPE      kb::float4
#define WEIGHT_TYPE     kb::float4

const uint16_t POSITION_COMPONENT_COUNT = sizeof(POSITION_TYPE) / sizeof(float);
const uint16_t NORMAL_COMPONENT_COUNT   = sizeof(NORMAL_TYPE)   / sizeof(float);
const uint16_t TANGENT_COMPONENT_COUNT  = sizeof(TANGENT_TYPE)  / sizeof(float);
const uint16_t TEXCOORD_COMPONENT_COUNT = sizeof(TEXCOORD_TYPE) / sizeof(float);
const uint16_t COLOR_COMPONENT_COUNT    = sizeof(COLOR_TYPE)    / sizeof(float);
const uint16_t JOINT_COMPONENT_COUNT    = sizeof(JOINT_TYPE)    / sizeof(float);
const uint16_t WEIGHT_COMPONENT_COUNT   = sizeof(WEIGHT_TYPE)   / sizeof(float);

using IndexType = uint32_t;

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
  IndexType position;
  IndexType normal;
  IndexType tangent;
  IndexType texcoords;
  IndexType colors;
  IndexType weights;
  IndexType joints;
};

struct IndexTriangle {
	VertexIndices vert[3];
};

struct VertexData {
  kb::array<POSITION_TYPE>  positions;
  kb::array<NORMAL_TYPE>    normals;
  kb::array<TANGENT_TYPE>   tangents;
  kb::array<TEXCOORD_TYPE>  texcoords;
  kb::array<COLOR_TYPE>     colors;
  kb::array<JOINT_TYPE>     joints;
  kb::array<WEIGHT_TYPE>    weights;
  
  kb::array<IndexTriangle> triangles;
};

template <typename T>
int32_t index_from(const T* f, const T* e) {
  if (f == nullptr || e == nullptr || e < f) return -1;
  return e - f;
}

void print_help(const char* error = nullptr) {
  if (error != nullptr) printf("%s\n\n", error);

  printf(
    "Kimberlite geomc\n"
    "\tUsage: geomc --input <file> --output <file> --index_size <int>\n" 
    "\tSet data to export with\n" 
    "\t\t--position\n"
    "\t\t--normal\n"
    "\t\t--tangent\n"
    "\t\t--texcoord\n"
    "\t\t--color\n"
    "\t\t--joints\n"
    "\t\t--weights\n"
  );
}

kb_xform get_node_xform(cgltf_node* node) {
  kb_xform xform;
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

  kb_stream*      rwops_in          = nullptr;
  kb_stream*      rwops_out         = nullptr;
  const char*     in_filepath       = nullptr;
  const char*     out_filepath      = nullptr;

  MeshParseData*  parse_meshes      = nullptr;
  VertexData      vertex_data       = {};

  bool            export_position   = false;
  bool            export_normal     = false;
  bool            export_tangent    = false;
  bool            export_texcoord   = false;
  bool            export_color      = false;
  bool            export_joints     = false;
  bool            export_weights    = false;

  int             index_size        = 0;
  uint64_t        max_prim_size     = 0;

  // Output geom
  kb_geometry_data geom = {};

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

  rwops_in = kb_stream_open_file(in_filepath, KB_FILE_MODE_READ);
  if (!rwops_in) {
    print_help("Unable to open input file");
    goto end;
  }

  rwops_out = kb_stream_open_file(out_filepath, KB_FILE_MODE_WRITE);
  if (!rwops_out) {
    print_help("Unable to open output file");
    goto end;
  }

  kb_cliargs_get_int(&args, &index_size, "index_size");
  if (index_size != 32) {
    print_help("Please specify index size with --index_size. Only 32 is supported");
    goto end;
  }
    
  max_prim_size   = ((uint64_t) 1 << index_size) - 1;
  geom.index_size = index_size / 8;
  
  export_position = kb_cliargs_has(&args, "position");
  export_normal   = kb_cliargs_has(&args, "normal");
  export_tangent  = kb_cliargs_has(&args, "tangent");
  export_texcoord = kb_cliargs_has(&args, "texcoord");
  export_color    = kb_cliargs_has(&args, "color");
  export_joints   = kb_cliargs_has(&args, "joints");
  export_weights  = kb_cliargs_has(&args, "weights");

  if (!(export_position || export_normal || export_tangent || export_texcoord || export_color || export_joints || export_weights)) {
    print_help("Nothing to export");
    goto end;
  }

  // Load
  input_size = kb_stream_size(rwops_in);
  input_data = KB_DEFAULT_ALLOC(input_size);
  kb_stream_read(rwops_in, input_data, 1, input_size);

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
    }

    // TODO: Animations
    for (cgltf_size anim_i = 0; anim_i < gltf_data->animations_count; ++anim_i) {
      cgltf_animation* src = &gltf_data->animations[anim_i];
    }

    // Node data
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

    // Mesh Data
    for (cgltf_size mesh_i = 0; mesh_i < geom.mesh_count; ++mesh_i) {
      parse_meshes[mesh_i] = {};
      geom.meshes[mesh_i]  = {};

      cgltf_mesh*     src = &gltf_data->meshes[mesh_i];
      MeshParseData*  dst = &parse_meshes[mesh_i];
      kb_strcpy(dst->name, src->name);

      for (cgltf_size prim_i = 0; prim_i < src->primitives_count; ++prim_i) {
        cgltf_primitive* prim = &src->primitives[prim_i];

        uint32_t prim_first_triangle_index  = vertex_data.triangles.count(); //kb_array_count(&vertex_data.triangles);
        uint32_t prim_first_position_index  = vertex_data.positions.count(); //kb_array_count(&vertex_data.positions);
        uint32_t prim_first_normal_index    = vertex_data.normals.count(); //kb_array_count(&vertex_data.normals);
        uint32_t prim_first_tangent_index   = vertex_data.tangents.count(); //kb_array_count(&vertex_data.tangents);
        uint32_t prim_first_texcoord_index  = vertex_data.texcoords.count(); //kb_array_count(&vertex_data.texcoords);
        uint32_t prim_first_color_index     = vertex_data.colors.count(); //kb_array_count(&vertex_data.colors);
        uint32_t prim_first_joints_index    = vertex_data.joints.count(); //kb_array_count(&vertex_data.joints);
        uint32_t prim_first_weights_index   = vertex_data.weights.count(); //kb_array_count(&vertex_data.weights);

        // Vertex data
        for (cgltf_size attrib_i = 0; attrib_i < prim->attributes_count; ++attrib_i) {
          cgltf_attribute*  attrib              = &prim->attributes[attrib_i];
          cgltf_accessor*   accessor            = attrib->data;
          cgltf_size        accessor_count      = accessor->count;
          cgltf_size        accessor_stride     = accessor->stride;
          cgltf_size        accessor_elem_size  = cgltf_num_components(accessor->type);
          
          if (attrib->index > 0) {
            printf("ERROR: Only attrib index 0 is currently supported. Sorry!\n");
            continue;
          }

          switch (attrib->type) {
            case cgltf_attribute_type_position: {
              uint32_t start = vertex_data.positions.count();
              vertex_data.positions.resize(start + accessor_count);

              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.positions.at(start + i), accessor_elem_size);

              }
            } break;

            case cgltf_attribute_type_normal: {
              uint32_t start = vertex_data.normals.count();
              vertex_data.normals.resize(start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.normals.at(start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_tangent: {
              uint32_t start = vertex_data.tangents.count();
              vertex_data.tangents.resize(start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.tangents.at(start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_texcoord: {
              uint32_t start = vertex_data.texcoords.count();
              vertex_data.texcoords.resize(start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.texcoords.at(start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_color: {
              uint32_t start = vertex_data.colors.count();

              vertex_data.colors.resize(start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.colors.at(start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_joints: {
              uint32_t start = vertex_data.joints.count();

              vertex_data.joints.resize(start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.joints.at(start + i), accessor_elem_size);
              }
            } break;

            case cgltf_attribute_type_weights: {
              uint32_t start = vertex_data.weights.count();

              vertex_data.weights.resize(start + accessor_count);
              
              for (uint32_t i = 0; i < accessor_count; i++) {
                cgltf_accessor_read_float(accessor, i, (cgltf_float*) &vertex_data.weights.at(start + i), accessor_elem_size);
              }
            } break;

            default: {} break;
          }
        }

        bool has_position = export_position && prim_first_position_index < vertex_data.positions.count();
        bool has_normal   = export_normal   && prim_first_normal_index   < vertex_data.normals.count();
        bool has_tangent  = export_tangent  && prim_first_tangent_index  < vertex_data.tangents.count();
        bool has_texcoord = export_texcoord && prim_first_texcoord_index < vertex_data.texcoords.count();
        bool has_color    = export_color    && prim_first_color_index    < vertex_data.colors.count();
        bool has_joints   = export_joints   && prim_first_joints_index   < vertex_data.joints.count();
        bool has_weights  = export_weights  && prim_first_weights_index  < vertex_data.weights.count();

        // Indices
        if (prim->indices) {
          cgltf_size index_count = prim->indices->count;
          for (cgltf_size i = 0; i < index_count; i += 3) { // Tri
            vertex_data.triangles.resize(vertex_data.triangles.count() + 1);
            IndexTriangle& tri = vertex_data.triangles.back();
            
            
            for (int v = 0; v < 3; ++v) {
              uint32_t vertex_index = cgltf_accessor_read_index(prim->indices, v + i);
          
              tri.vert[v].position  = has_position  ? prim_first_position_index + vertex_index : (IndexType) -1;
              tri.vert[v].normal    = has_normal    ? prim_first_normal_index   + vertex_index : (IndexType) -1;
              tri.vert[v].tangent   = has_tangent   ? prim_first_tangent_index  + vertex_index : (IndexType) -1;
              tri.vert[v].texcoords = has_texcoord  ? prim_first_texcoord_index + vertex_index : (IndexType) -1;
              tri.vert[v].colors    = has_color     ? prim_first_color_index    + vertex_index : (IndexType) -1;
              tri.vert[v].joints    = has_joints    ? prim_first_joints_index   + vertex_index : (IndexType) -1;
              tri.vert[v].weights   = has_weights   ? prim_first_weights_index  + vertex_index : (IndexType) -1;
            }

          }
        }

        // Split to primitives
        uint32_t tri_count = vertex_data.triangles.count() - prim_first_triangle_index;

        uint32_t tri_ind = 0;

        while (tri_ind < tri_count) {
          uint32_t tris_remaining = tri_count - tri_ind;
          uint32_t prim_tri_count = kb_int_min(tris_remaining, max_prim_size / 3);

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

    for (uint64_t tri_i = 0; tri_i < vertex_data.triangles.count(); ++tri_i) {
      IndexTriangle& tri = vertex_data.triangles.at(tri_i);

      for (uint32_t i = 0; i < 3; ++i) {
        has_position  |= tri.vert[i].position   != (IndexType) -1;
        has_normal    |= tri.vert[i].normal     != (IndexType) -1;
        has_tangent   |= tri.vert[i].tangent    != (IndexType) -1;
        has_texcoord  |= tri.vert[i].texcoords  != (IndexType) -1;
        has_color     |= tri.vert[i].colors     != (IndexType) -1;
        has_weights   |= tri.vert[i].weights    != (IndexType) -1;
        has_joints    |= tri.vert[i].joints     != (IndexType) -1;
      }
    }
    
    int32_t attrib_position  = -1;
    int32_t attrib_normal    = -1;
    int32_t attrib_tangent   = -1;
    int32_t attrib_texcoord  = -1;
    int32_t attrib_color     = -1;
    int32_t attrib_joints    = -1;
    int32_t attrib_weights   = -1;

    kb_vertex_layout vertex_layout;

    kb_vertex_layout_begin(&vertex_layout);
      if (has_position) attrib_position = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, POSITION_COMPONENT_COUNT);
      if (has_normal)   attrib_normal   = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, NORMAL_COMPONENT_COUNT);
      if (has_tangent)  attrib_tangent  = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, TANGENT_COMPONENT_COUNT);
      if (has_texcoord) attrib_texcoord = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, TEXCOORD_COMPONENT_COUNT);
      if (has_color)    attrib_color    = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, COLOR_COMPONENT_COUNT);
      if (has_weights)  attrib_weights  = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, WEIGHT_COMPONENT_COUNT);
      if (has_joints)   attrib_joints   = kb_vertex_layout_add(&vertex_layout, KB_ATTRIB_FLOAT, JOINT_COMPONENT_COUNT);
    kb_vertex_layout_end(&vertex_layout);

    uint32_t vertex_stride = kb_vertex_layout_stride(&vertex_layout);

    uint64_t max_vertex_count = vertex_data.triangles.count() * 3;
    uint64_t max_index_count  = vertex_data.triangles.count() * 3;

    uint8_t*  geom_vert_data  = (uint8_t*)    KB_DEFAULT_ALLOC(max_vertex_count * vertex_stride);
    IndexType* geom_ind_data  = (IndexType*)  KB_DEFAULT_ALLOC(sizeof(IndexType) * max_index_count);
    
    uint64_t vertex_count = 0;
    uint64_t index_count  = 0;
    
    for (uint32_t i = 0; i < geom.mesh_count; ++i) {
      MeshParseData* mesh = &parse_meshes[i];

      // Init output mesh
      kb_mesh_data* mesh_out    = &geom.meshes[i];
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
          IndexTriangle& tri = vertex_data.triangles.at(tri_i);

          for (uint32_t v = 0; v < 3; ++v) {
            const VertexIndices& vert = tri.vert[v];

            if (has_position) {
              uint32_t idx = vert.position == UINT32_MAX ? 0 : vert.position;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_position);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_position);
              
              kb_memcpy_with_stride(prim_vert_data, &vertex_data.positions.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }

            if (has_normal) {
              uint32_t idx = vert.normal == UINT32_MAX ? 0 : vert.normal;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_normal);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_normal);

              kb_memcpy_with_stride(prim_vert_data, &vertex_data.normals.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_tangent) {
              uint32_t idx = vert.tangent == UINT32_MAX ? 0 : vert.tangent;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_tangent);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_tangent);

              kb_memcpy_with_stride(prim_vert_data, &vertex_data.tangents.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }

            if (has_texcoord) {
              uint32_t idx = vert.texcoords == UINT32_MAX ? 0 : vert.texcoords;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_texcoord);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_texcoord);

              kb_memcpy_with_stride(prim_vert_data, &vertex_data.texcoords.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_color) {
              uint32_t idx = vert.colors == UINT32_MAX ? 0 : vert.colors;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_color);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_color);

              kb_memcpy_with_stride(prim_vert_data, &vertex_data.colors.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_weights) {
              uint32_t idx = vert.weights == UINT32_MAX ? 0 : vert.weights;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_weights);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_weights);

              kb_memcpy_with_stride(prim_vert_data, &vertex_data.weights.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
            }
            
            if (has_joints) {
              uint32_t idx = vert.joints == UINT32_MAX ? 0 : vert.joints;
              uint32_t attrib_size   = kb_vertex_layout_size(&vertex_layout, attrib_joints);
              uint32_t attrib_offset = kb_vertex_layout_offset(&vertex_layout, attrib_joints);

              kb_memcpy_with_stride(prim_vert_data, &vertex_data.joints.at(idx), attrib_size, prim_vert, vertex_stride, attrib_offset);
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

  printf("All done! Total optimization time: %f\n", double(total_optim_time) / double(kb_time_get_frequency()));
    
  //#####################################################################################################################
  // Export
  //#####################################################################################################################
    
  kb_geometry_data_write(&geom, rwops_out);

  exit_val = EXIT_SUCCESS;
end:
  kb_stream_close(rwops_in);
  kb_stream_close(rwops_out);
  
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
