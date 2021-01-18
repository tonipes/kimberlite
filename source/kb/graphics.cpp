// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/graphics.h>

#include <kb/platform.h>
#include <kb/time.h>
#include <kb/sampler.h>
#include <kb/log.h>

typedef struct pipeline_info {
  kb_uniform_layout         uniform_layout;
  kb_vertex_layout_info     vertex_layout;
  uint32_t                  renderpass;
} pipeline_info;

typedef struct kb_pipe_attachment {
  kb_format                 format;
  kb_texture                texture;
  kb_texture_usage          usage;
  bool                      surface_proxy;
  bool                      resize_with_surface;
} kb_pipe_attachment;

typedef struct graphics_pipe_info {
  kb_pipe_attachment        attachments[KB_CONFIG_MAX_PIPE_ATTACHMENTS];
  int32_t                   attachment_count;
  int32_t                   pass_count;
  kb_renderpass_info        passes[KB_CONFIG_MAX_PASSES];
} graphics_pipe_info;

typedef struct kb_encoder_frame {
  kb_pipeline               pipeline;
  kb_vertex_buffer_binding  vertex_buffer_bindings[KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS];
  kb_index_buffer_binding   index_buffer;
  kb_texture_binding        vert_texture_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        vert_uniform_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_texture_binding        frag_texture_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        frag_uniform_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_encoder_frame;

typedef struct kb_encoder_state {
  uint32_t                  stack_pos;
  uint32_t                  draw_call_count;
  kb_encoder_frame          stack[KB_CONFIG_GIZMO_STACK_SIZE];
  kb_graphics_call*         draw_calls;
} kb_encoder_state;

typedef struct kb_encoder_pool {
  uint16_t                  count;
  kb_encoder_state          states[KB_CONFIG_MAX_ENCODERS];
} kb_encoder_pool;

typedef struct kb_transient_buffer {
  kb_buffer                 buffer;
  uint64_t                  position;
} kb_transient_buffer;

kb_encoder_pool*    encoder_pools;
graphics_pipe_info* graphics_pipe;

uint32_t            resource_slot;
kb_int2             current_extent;
kb_graphics_call*   draw_call_cache[KB_CONFIG_MAX_RENDERPASSES];
uint32_t            draw_call_cache_pos[KB_CONFIG_MAX_RENDERPASSES];

kb_transient_buffer transient_buffers[KB_CONFIG_MAX_FRAMES_IN_FLIGHT];

kb_encoder_pool& current_encoder_pool() {
  return encoder_pools[kb_graphics_get_current_resource_slot()];
}

kb_encoder_state& current_encoder_state(kb_encoder encoder) {
  return current_encoder_pool().states[kb_to_arr(encoder)];
}

kb_encoder_frame& current_encoder_frame(kb_encoder_state& state) {
  return state.stack[state.stack_pos];
}

kb_encoder_frame& current_encoder_frame(kb_encoder encoder) {
  return current_encoder_frame(current_encoder_state(encoder));
}

KB_RESOURCE_ALLOC_FUNC_DEF  (buffer,        kb_buffer,        kb_buffer_create_info,        KB_CONFIG_MAX_BUFFERS);
KB_RESOURCE_ALLOC_FUNC_DEF  (texture,       kb_texture,       kb_texture_create_info,       KB_CONFIG_MAX_TEXTURES);
KB_RESOURCE_ALLOC_FUNC_DEF  (pipeline,      kb_pipeline,      kb_pipeline_create_info,      KB_CONFIG_MAX_PROGRAMS);

KB_RESOURCE_DATA_HASHED_DEF (buffer,        kb_buffer);
KB_RESOURCE_DATA_HASHED_DEF (texture,       kb_texture);
KB_RESOURCE_DATA_HASHED_DEF (pipeline,      kb_pipeline);

KB_RESOURCE_STORAGE_DEF     (pipeline_info, kb_pipeline,      pipeline_info,      KB_CONFIG_MAX_PROGRAMS);

#define COMPARE_VALUE(_a, _b) if (_a < _b) return -1; if (_b < _a) return 1;

void reset_encoder_state(kb_encoder_state& state) {
  state.stack_pos = 0;
  state.draw_call_count = 0;
}

void construct_encoder_pools() {
  encoder_pools = KB_DEFAULT_ALLOC_TYPE(kb_encoder_pool, KB_CONFIG_MAX_FRAMES_IN_FLIGHT);

  for (int pool_i = 0; pool_i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; ++pool_i) {
    for (int state_i = 0; state_i < KB_CONFIG_MAX_ENCODERS; ++state_i) {
      encoder_pools[pool_i].states[state_i].draw_calls = KB_DEFAULT_ALLOC_TYPE(kb_graphics_call, KB_CONFIG_MAX_DRAW_CALLS);
    }
  }
}

void destruct_encoder_pools() {
  for (int pool_i = 0; pool_i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; ++pool_i) {
    for (int state_i = 0; state_i < KB_CONFIG_MAX_ENCODERS; ++state_i) {
      KB_DEFAULT_FREE(encoder_pools[pool_i].states[state_i].draw_calls);
    }
  }

  KB_DEFAULT_FREE(encoder_pools);
}

static int draw_call_compare(const void* a, const void* b) {
  const kb_graphics_call* aa = (kb_graphics_call*) a;
  const kb_graphics_call* bb = (kb_graphics_call*) b;

  COMPARE_VALUE(aa->renderpass,               bb->renderpass);
  COMPARE_VALUE(aa->pipeline.idx,             bb->pipeline.idx);
  COMPARE_VALUE(aa->index_buffer.buffer.idx,  bb->index_buffer.buffer.idx);
  
  for (uint32_t i = 0; i < KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS; ++i) {
    COMPARE_VALUE(aa->vertex_buffer_bindings[i].buffer.idx, bb->vertex_buffer_bindings[i].buffer.idx);
    COMPARE_VALUE(aa->vertex_buffer_bindings[i].offset,     bb->vertex_buffer_bindings[i].offset);
  }
  
  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    COMPARE_VALUE(aa->vert_uniform_bindings[i].buffer.idx,  bb->vert_uniform_bindings[i].buffer.idx);
    COMPARE_VALUE(aa->vert_uniform_bindings[i].index,       bb->vert_uniform_bindings[i].index);
    COMPARE_VALUE(aa->vert_uniform_bindings[i].offset,      bb->vert_uniform_bindings[i].offset);

    COMPARE_VALUE(aa->frag_uniform_bindings[i].buffer.idx,  bb->frag_uniform_bindings[i].buffer.idx);
    COMPARE_VALUE(aa->frag_uniform_bindings[i].index,       bb->frag_uniform_bindings[i].index);
    COMPARE_VALUE(aa->frag_uniform_bindings[i].offset,      bb->frag_uniform_bindings[i].offset);

    COMPARE_VALUE(aa->vert_texture_bindings[i].texture.idx, bb->vert_texture_bindings[i].texture.idx);
    COMPARE_VALUE(aa->vert_texture_bindings[i].index,       bb->vert_texture_bindings[i].index);

    COMPARE_VALUE(aa->frag_texture_bindings[i].texture.idx, bb->frag_texture_bindings[i].texture.idx);
    COMPARE_VALUE(aa->frag_texture_bindings[i].index,       bb->frag_texture_bindings[i].index);
  }

  return 0;
}

KB_INTERNAL kb_transient_buffer& get_current_transient_buffer() {
  return transient_buffers[resource_slot];
}

KB_INTERNAL void acquire_frame_resources() {
  resource_slot = (resource_slot + 1) % KB_CONFIG_MAX_FRAMES_IN_FLIGHT;
  get_current_transient_buffer().position = 0;
}

KB_API uint32_t kb_graphics_get_current_resource_slot() {
  return resource_slot;
}

KB_API kb_buffer kb_graphics_transient_buffer() {
  return get_current_transient_buffer().buffer;
}
  
KB_API int64_t kb_graphics_transient_alloc(uint64_t size, kb_buffer_usage usage) {
  kb_transient_buffer& buffer = get_current_transient_buffer();
  
  // TODO: Calculate align from usage
  uint64_t align = 256;

  size_t pos = kb_align_up(buffer.position, align);
  buffer.position = pos + size;
  
  return pos;
}

KB_API void kb_graphics_init(const kb_graphics_init_info info) {
  kb_platform_graphics_init(info);

  for (uint32_t pass_i = 0; pass_i < KB_CONFIG_MAX_RENDERPASSES; ++pass_i) {
    draw_call_cache_pos[pass_i] = 0;
    draw_call_cache[pass_i]     = KB_DEFAULT_ALLOC_TYPE(kb_graphics_call, KB_CONFIG_MAX_DRAW_CALLS);
  }
  
  // Transient buffers
  for (uint32_t frame_i = 0; frame_i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; frame_i++) {
    transient_buffers[frame_i] = {};
    transient_buffers[frame_i].position = 0;
    transient_buffers[frame_i].buffer = kb_buffer_create({
      .rwops  = NULL,
      .size   = KB_CONFIG_TRANSIENT_BUFFER_SIZE,
      .usage  = KB_BUFFER_USAGE_VERTEX_BUFFER | KB_BUFFER_USAGE_INDEX_BUFFER | KB_BUFFER_USAGE_UNIFORM_BUFFER | KB_BUFFER_USAGE_CPU_WRITE
    });
  }

  graphics_pipe = KB_DEFAULT_ALLOC_TYPE(graphics_pipe_info, 1);
  current_extent = kb_platform_graphics_surface_extent();
  graphics_pipe->attachment_count = info.pipe.attachment_count;
  graphics_pipe->pass_count = info.pipe.pass_count;

  // Attachments
  for (uint32_t attachment_i = 0; attachment_i < info.pipe.attachment_count; ++attachment_i) {
    const kb_attachment_info& attachment_info = info.pipe.attachments[attachment_i];
    
    kb_pipe_attachment& attachment = graphics_pipe->attachments[attachment_i];
    attachment = {};
    
    if (attachment_info.surface_proxy) {
      // Drawable proxy. Mark as such and do not create a texture
      attachment.surface_proxy = true;
    } else {
      // Regular attachment. Create a texture
      KB_ASSERT(attachment_info.usage & KB_TEXTURE_USAGE_RENDER_TARGET, "Graphics pipeline attachments must be render targets");
      
      attachment.resize_with_surface = attachment_info.use_surface_size;
      attachment.format = attachment_info.texture.format;
      attachment.usage = attachment_info.usage;
      attachment.texture = kb_texture_create({
        .rwops    = NULL,
        .usage    = attachment_info.usage,
        .texture  = {
          .format = attachment_info.texture.format,
          .width  = attachment_info.use_surface_size ? current_extent.x : attachment_info.texture.width,
          .height = attachment_info.use_surface_size ? current_extent.y : attachment_info.texture.height,
        },
        .sampler = {
          .min_filter     = KB_FILTER_NEAREST,
          .mag_filter     = KB_FILTER_NEAREST,
          .address_mode_u = KB_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
          .address_mode_v = KB_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
          .address_mode_w = KB_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
          .anisotropy     = 0.0f,
        },
        .mipmaps = false,
      });
    }
  }
  
  for (uint32_t pass_i = 0; pass_i < info.pipe.pass_count; ++pass_i) {
    graphics_pipe->passes[pass_i] = {};
    graphics_pipe->passes[pass_i] = info.pipe.passes[pass_i];
  }

  construct_encoder_pools();
  
  acquire_frame_resources();
}

KB_API void kb_graphics_deinit() {
  kb_pipeline_purge();
  kb_texture_purge();

  kb_platform_graphics_deinit();

  for (uint32_t pass_i = 0; pass_i < KB_CONFIG_MAX_RENDERPASSES; ++pass_i) {
    KB_DEFAULT_FREE(draw_call_cache[pass_i]);
  }
  
  destruct_encoder_pools();
}

KB_API void kb_graphics_run_encoders() {
  // Fill caches
  for (uint32_t encoder_i = 0; encoder_i < current_encoder_pool().count; ++encoder_i) {
    kb_encoder_state& state = current_encoder_pool().states[encoder_i];
    for (uint32_t call_i = 0; call_i < state.draw_call_count; ++call_i) {
      kb_graphics_call& call = state.draw_calls[call_i];
      KB_ASSERT_VALID(call.pipeline);
      uint32_t renderpass = pipeline_info_ref(call.pipeline)->renderpass;
      
      draw_call_cache[renderpass][draw_call_cache_pos[renderpass]++] = call;
    }
  }
    
  // Run passes
  for (uint32_t pass_i = 0; pass_i < KB_CONFIG_MAX_RENDERPASSES; ++pass_i) {
    if (draw_call_cache_pos[pass_i] <= 0) continue;
    
    // Sort calls
    kb_sort(draw_call_cache[pass_i], draw_call_cache_pos[pass_i], sizeof(kb_graphics_call), draw_call_compare);
    
    kb_platform_graphics_submit_calls(pass_i, draw_call_cache[pass_i], draw_call_cache_pos[pass_i]);
    draw_call_cache_pos[pass_i] = 0;
  }
}

KB_API void kb_graphics_frame() {
  kb_int2 extent = kb_platform_graphics_surface_extent();

  if (extent.x != current_extent.x || extent.y != current_extent.y) {
    current_extent = extent;
    kb_log_debug("Surface resized ({} {})", current_extent.x, current_extent.y);

    // Resize attachments
    for (uint32_t attachment_i = 0; attachment_i < graphics_pipe->attachment_count; ++attachment_i) {
      kb_texture texture = graphics_pipe->attachments[attachment_i].texture;
      if (!KB_IS_VALID(graphics_pipe->attachments[attachment_i].texture)) continue;
      if (!graphics_pipe->attachments[attachment_i].resize_with_surface) continue;
      
      kb_platform_graphics_texture_destruct(texture);
      
      kb_platform_graphics_texture_construct(texture, {
        .usage  = graphics_pipe->attachments[attachment_i].usage,
        .texture = {
          .width  = (uint32_t) current_extent.x,
          .height = (uint32_t) current_extent.y,
          .format = graphics_pipe->attachments[attachment_i].format,
        }
      });
    }
  }
  
  kb_platform_graphics_frame();
  
  // Reset pools
  kb_encoder_pool& current_pool = current_encoder_pool();
  for (int i = 0; i < current_pool.count; ++i) {
    reset_encoder_state(current_pool.states[i]);
  }
  current_pool.count = 0;
  
  acquire_frame_resources();
}

KB_API kb_texture kb_graphics_pipe_attachment_texture(uint32_t attachment) {
  KB_ASSERT_NOT_NULL(graphics_pipe);

  return graphics_pipe->attachments[attachment].texture;
}

KB_API kb_format kb_graphics_pipe_attachment_format(uint32_t attachment) {
  KB_ASSERT_NOT_NULL(graphics_pipe);

  return graphics_pipe->attachments[attachment].format;
}

KB_API kb_renderpass_info* kb_graphics_get_renderpass_info(uint32_t pass) {
  KB_ASSERT_NOT_NULL(graphics_pipe);
  KB_ASSERT(graphics_pipe->pass_count > pass, "Invalid Pass");

  return &graphics_pipe->passes[pass];
}

KB_API bool kb_graphics_pipe_attachment_surface_proxy(uint32_t attachment) {
  KB_ASSERT_NOT_NULL(graphics_pipe);
  KB_ASSERT(graphics_pipe->attachment_count > attachment, "Invalid Attachment");

  return graphics_pipe->attachments[attachment].surface_proxy;
}

KB_API void kb_graphics_pipe_destruct(kb_graphics_pipe handle) {
  KB_ASSERT_VALID(handle);

  kb_log_debug("kb_graphics_pipe_destruct");
}

KB_API void kb_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {
  KB_ASSERT_VALID(handle);
  
  pipeline_info_ref(handle)->uniform_layout  = info.uniform_layout;
  pipeline_info_ref(handle)->vertex_layout   = info.vertex_layout;
  pipeline_info_ref(handle)->renderpass      = info.renderpass;

  kb_platform_graphics_pipeline_construct(handle, info);
  kb_pipeline_set_initialized(handle, true);
}

KB_API void kb_pipeline_destruct(kb_pipeline handle) {
  KB_ASSERT_VALID(handle);

  kb_platform_graphics_pipeline_destruct(handle);
  kb_pipeline_set_initialized(handle, false);
}

KB_API void kb_texture_construct(kb_texture handle, const kb_texture_create_info info) {
  KB_ASSERT_VALID(handle);

  kb_platform_graphics_texture_construct(handle, info);
  kb_texture_set_initialized(handle, true);
}

KB_API void kb_texture_destruct(kb_texture handle) {
  KB_ASSERT_VALID(handle);

  kb_platform_graphics_texture_destruct(handle);
  kb_texture_set_initialized(handle, false);
}

KB_API void kb_buffer_construct(kb_buffer handle, const kb_buffer_create_info info) {
  KB_ASSERT_VALID(handle);

  kb_platform_graphics_buffer_construct(handle, info);
  kb_buffer_set_initialized(handle, true);
}

KB_API void kb_buffer_destruct(kb_buffer handle) {
  KB_ASSERT_VALID(handle);

  kb_platform_graphics_buffer_destruct(handle);
  kb_buffer_set_initialized(handle, false);
}

KB_API kb_uniform_slot kb_uniform_get_slot(const kb_uniform_layout* layout, kb_hash hash, kb_binding_type type) {
  KB_ASSERT_NOT_NULL(layout);

  kb_uniform_slot out {};
  
  if (type == KB_BINDING_TYPE_UNIFORM_BUFFER) {
    for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
      
      if (layout->vert_ubos[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_VERTEX);
        out.type = KB_BINDING_TYPE_UNIFORM_BUFFER;
        out.vert_index = layout->vert_ubos[i].slot;
      }
      
      if (layout->frag_ubos[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_FRAGMENT);
        out.type = KB_BINDING_TYPE_UNIFORM_BUFFER;
        out.frag_index = layout->frag_ubos[i].slot;
      }

    }
  }

  if (type == KB_BINDING_TYPE_TEXTURE) {
    for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
      
      if (layout->vert_textures[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_VERTEX);
        out.type = KB_BINDING_TYPE_TEXTURE;
        out.vert_index = layout->vert_textures[i].slot;
      }
      
      if (layout->frag_textures[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_FRAGMENT);
        out.type = KB_BINDING_TYPE_TEXTURE;
        out.frag_index = layout->frag_textures[i].slot;
      }

    }
  }
  
  return out;
}

KB_API kb_encoder kb_encoder_begin() {
  kb_encoder_pool& pool = current_encoder_pool();

  uint32_t slot = pool.count++;
  kb_encoder encoder = KB_HANDLE_FROM_ARRAY(slot);
  kb_encoder_state& state = current_encoder_state(encoder);
  
  kb_encoder_reset_frame(encoder);

  return encoder;
}

KB_API void kb_encoder_reset_frame(kb_encoder encoder) {
  KB_ASSERT_VALID(encoder);

  current_encoder_frame(encoder) = {};
}

KB_API void kb_encoder_end(kb_encoder encoder) {
  KB_ASSERT_VALID(encoder);
}

KB_API void kb_encoder_push(kb_encoder encoder) { 
  KB_ASSERT_VALID(encoder);

  kb_encoder_state& state = current_encoder_state(encoder);
  KB_ASSERT(state.stack_pos < KB_COUNTOF(state.stack), "Encoder stack overflow!");

  state.stack_pos++;
  state.stack[state.stack_pos] = state.stack[state.stack_pos - 1];
}

KB_API void kb_encoder_pop(kb_encoder encoder) {
  KB_ASSERT_VALID(encoder);

  kb_encoder_state& state = current_encoder_state(encoder);
  KB_ASSERT(state.stack_pos > 0, "Can't pop when stack at 0");

  --state.stack_pos;
}
//
//KB_API void kb_encoder_bind_renderpass(kb_encoder encoder, uint32_t renderpass) {
//  KB_ASSERT_VALID(encoder);
//
//  current_encoder_frame(encoder).renderpass = renderpass;
//}

KB_API void kb_encoder_bind_pipeline(kb_encoder encoder, kb_pipeline pipeline) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(pipeline);

  current_encoder_frame(encoder).pipeline = pipeline;
}

KB_API void kb_encoder_bind_buffer(kb_encoder encoder, uint32_t slot, kb_buffer buffer, uint64_t offset) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(buffer);

  current_encoder_frame(encoder).vertex_buffer_bindings[slot].buffer = buffer;
  current_encoder_frame(encoder).vertex_buffer_bindings[slot].offset = offset;
}

KB_API void kb_encoder_bind_index_buffer(kb_encoder encoder, kb_buffer buffer, uint64_t offset, kb_index_type type) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(buffer);

  current_encoder_frame(encoder).index_buffer.buffer      = buffer;
  current_encoder_frame(encoder).index_buffer.offset      = offset;
  current_encoder_frame(encoder).index_buffer.index_type  = type;
}

KB_API void kb_encoder_bind_texture(kb_encoder encoder, const kb_uniform_slot slot, kb_texture texture) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(texture);

  KB_ASSERT(slot.vert_index < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Slot index too large");
  KB_ASSERT(slot.frag_index < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Slot index too large");

  if (slot.stage & KB_SHADER_STAGE_VERTEX) {
    kb_texture_binding& binding = current_encoder_frame(current_encoder_state(encoder)).vert_texture_bindings[slot.vert_index];
    binding.index    = slot.vert_index;
    binding.texture  = texture;
  }
  
  if (slot.stage & KB_SHADER_STAGE_FRAGMENT) {
    kb_texture_binding& binding = current_encoder_frame(current_encoder_state(encoder)).frag_texture_bindings[slot.frag_index]; 
    binding.index    = slot.frag_index;
    binding.texture  = texture;
  }
}

KB_API void kb_encoder_bind_uniform(kb_encoder encoder, const kb_uniform_slot slot, const void* data, uint64_t size) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_NOT_NULL(data);

  KB_ASSERT(slot.vert_index < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Slot index too large");
  KB_ASSERT(slot.frag_index < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Slot index too large");

  uint64_t data_size = size;

  if (data_size == 0) return;
  KB_ASSERT_NOT_NULL(data);

  kb_buffer buffer = kb_graphics_transient_buffer();
  int64_t buffer_offset = kb_graphics_transient_alloc(data_size, KB_BUFFER_USAGE_UNIFORM_BUFFER);
  
  void* ptr = kb_platform_graphics_buffer_mapped(buffer, buffer_offset);
  
  if (!ptr) {
    kb_log_error("Failed to allocate transient data for uniform buffer binding");
    return;
  }
  
  kb_memcpy(ptr, data, data_size);
  
  if (slot.stage & KB_SHADER_STAGE_VERTEX) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).vert_uniform_bindings[slot.vert_index];
    binding->index  = slot.vert_index;
    binding->size   = data_size;
    binding->offset = buffer_offset,
    binding->buffer = buffer;
  }
  
  if (slot.stage & KB_SHADER_STAGE_FRAGMENT) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).frag_uniform_bindings[slot.frag_index]; 
    binding->index  = slot.frag_index;
    binding->size   = data_size;
    binding->offset = buffer_offset,
    binding->buffer = buffer;
  }
}

KB_API void kb_encoder_submit(kb_encoder encoder, uint32_t first_index, uint32_t first_vertex, uint32_t index_count, uint32_t instance_count) {
  KB_ASSERT_VALID(encoder);

  kb_encoder_state& state = current_encoder_state(encoder);
  kb_encoder_frame& frame = current_encoder_frame(encoder);
  
  KB_ASSERT(state.draw_call_count < KB_CONFIG_MAX_DRAW_CALLS, "Too many draw calls (KB_CONFIG_MAX_DRAW_CALLS)");
  
  kb_graphics_call& call = state.draw_calls[state.draw_call_count++];
  
  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++binding) {
    call.vert_uniform_bindings[binding] = frame.vert_uniform_bindings[binding];
    call.vert_texture_bindings[binding] = frame.vert_texture_bindings[binding];
    call.frag_uniform_bindings[binding] = frame.frag_uniform_bindings[binding];
    call.frag_texture_bindings[binding] = frame.frag_texture_bindings[binding];
  }

  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS; ++binding) {
    call.vertex_buffer_bindings[binding] = frame.vertex_buffer_bindings[binding];
  }

  call.pipeline             = frame.pipeline;
  call.index_buffer         = frame.index_buffer;
  call.info.first_vertex    = first_vertex;
  call.info.first_index     = first_index;
  call.info.index_count     = index_count;
  call.info.instance_count  = instance_count;
}

KB_API void* kb_graphics_get_buffer_mapped(kb_buffer buffer, uint64_t offset) {
  KB_ASSERT_VALID(buffer);

  return kb_platform_graphics_buffer_mapped(buffer, offset);
}

KB_API kb_int2 kb_graphics_get_extent() {    
  return current_extent;
}

KB_API float kb_graphics_get_aspect() {
  kb_int2 size = kb_graphics_get_extent();
  return float(size.x) / float(size.y);
}
