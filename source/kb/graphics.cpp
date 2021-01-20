// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation.h>

#include <kb/graphics.h>
#include <kb/platform.h>
#include <kb/log.h>

typedef struct pipeline_info {
  kb_uniform_layout         uniform_layout;
  uint32_t                  pass;
} pipeline_info;

typedef struct kb_pipe_attachment {
  kb_attachment_flags       flags;
  kb_format                 format;
  kb_texture                texture;
  kb_texture_usage          usage;
  kb_float2                 size;
} kb_pipe_attachment;

typedef struct graphics_pipe_info {
  kb_pipe_attachment        attachments[KB_CONFIG_MAX_PIPE_ATTACHMENTS];
  int32_t                   attachment_count;
  int32_t                   pass_count;
  kb_pass_info              passes[KB_CONFIG_MAX_PASSES];
} graphics_pipe_info;

typedef struct kb_encoder_frame {
  kb_pipeline               pipeline;
  kb_vertex_buffer_binding  vertex_buffer_bindings[KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS];
  kb_index_buffer_binding   index_buffer;
  kb_texture_binding        vertex_texture_bindings   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        vertex_uniform_bindings   [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_texture_binding        fragment_texture_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        fragment_uniform_bindings [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_texture_binding        compute_texture_bindings  [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_uniform_binding        compute_uniform_bindings  [KB_CONFIG_MAX_UNIFORM_BINDINGS];
} kb_encoder_frame;

typedef struct kb_encoder_state {
  uint32_t                  stack_pos;
  uint32_t                  draw_call_count;
  uint32_t                  compute_call_count;
  kb_encoder_frame          stack[KB_CONFIG_GIZMO_STACK_SIZE];
  kb_render_call*           draw_calls;
  kb_compute_call*          compute_calls;
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

kb_render_call*     draw_call_cache[KB_CONFIG_MAX_RENDERPASSES];
uint32_t            draw_call_cache_pos[KB_CONFIG_MAX_RENDERPASSES];

kb_compute_call*    compute_call_cache[KB_CONFIG_MAX_RENDERPASSES];
uint32_t            compute_call_cache_pos[KB_CONFIG_MAX_RENDERPASSES];

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
  state.compute_call_count = 0;
}

void construct_encoder_pools() {
  encoder_pools = KB_DEFAULT_ALLOC_TYPE(kb_encoder_pool, KB_CONFIG_MAX_FRAMES_IN_FLIGHT);

  for (int pool_i = 0; pool_i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; ++pool_i) {
    for (int state_i = 0; state_i < KB_CONFIG_MAX_ENCODERS; ++state_i) {
      encoder_pools[pool_i].states[state_i].draw_calls = KB_DEFAULT_ALLOC_TYPE(kb_render_call, KB_CONFIG_MAX_DRAW_CALLS);
      encoder_pools[pool_i].states[state_i].compute_calls = KB_DEFAULT_ALLOC_TYPE(kb_compute_call, KB_CONFIG_MAX_DRAW_CALLS);
      
    }
  }
}

void destruct_encoder_pools() {
  for (int pool_i = 0; pool_i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; ++pool_i) {
    for (int state_i = 0; state_i < KB_CONFIG_MAX_ENCODERS; ++state_i) {
      KB_DEFAULT_FREE(encoder_pools[pool_i].states[state_i].draw_calls);
      KB_DEFAULT_FREE(encoder_pools[pool_i].states[state_i].compute_calls);
    }
  }

  KB_DEFAULT_FREE(encoder_pools);
}

static int draw_call_compare(const void* a, const void* b) {
  const kb_render_call* aa = (kb_render_call*) a;
  const kb_render_call* bb = (kb_render_call*) b;

//  COMPARE_VALUE(aa->pass,                             bb->pass);
  COMPARE_VALUE(aa->pipeline.idx,                     bb->pipeline.idx);
  COMPARE_VALUE(aa->index_buffer.memory.buffer.idx,   bb->index_buffer.memory.buffer.idx);
  COMPARE_VALUE(aa->index_buffer.index_type,          bb->index_buffer.index_type);

  for (uint32_t i = 0; i < KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS; ++i) {
    COMPARE_VALUE(aa->vertex_buffer_bindings[i].memory.buffer.idx, bb->vertex_buffer_bindings[i].memory.buffer.idx);
    COMPARE_VALUE(aa->vertex_buffer_bindings[i].memory.offset,     bb->vertex_buffer_bindings[i].memory.offset);
  }
  
  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    COMPARE_VALUE(aa->vertex_uniform_bindings[i].memory.buffer.idx,   bb->vertex_uniform_bindings[i].memory.buffer.idx);
    COMPARE_VALUE(aa->vertex_uniform_bindings[i].memory.offset,       bb->vertex_uniform_bindings[i].memory.offset);

    COMPARE_VALUE(aa->fragment_uniform_bindings[i].memory.buffer.idx,   bb->fragment_uniform_bindings[i].memory.buffer.idx);
    COMPARE_VALUE(aa->fragment_uniform_bindings[i].memory.offset,       bb->fragment_uniform_bindings[i].memory.offset);

    COMPARE_VALUE(aa->vertex_texture_bindings[i].texture.idx, bb->vertex_texture_bindings[i].texture.idx);

    COMPARE_VALUE(aa->fragment_texture_bindings[i].texture.idx, bb->fragment_texture_bindings[i].texture.idx);
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

KB_API kb_buffer_memory kb_graphics_transient_alloc(uint64_t size, kb_buffer_usage usage) {
  kb_transient_buffer& buffer = get_current_transient_buffer();
  
  // TODO: Calculate align from usage
  uint64_t align = 256;

  size_t pos = kb_align_up(buffer.position, align);
  buffer.position = pos + size;
  
  return { 
    .buffer=buffer.buffer,
    .offset=pos,
  };
}

KB_API kb_buffer_memory kb_graphics_transient_write(const void* src, uint64_t size, kb_buffer_usage usage) {
  if (size == 0) return;
  
  kb_buffer_memory memory = kb_graphics_transient_alloc(size, usage);
  
  if (src != NULL) {
    void* dst = kb_platform_graphics_buffer_mapped(memory);
    kb_memcpy(dst, src, size);
  }
  
  return memory;
}

kb_int2 calculate_attachment_size(kb_int2 extent, kb_float2 size, kb_attachment_flags flags) {
  if (flags & KB_ATTACHMENT_FLAGS_USE_SURFACE_SIZE) return extent;
  if (flags & KB_ATTACHMENT_FLAGS_SIZE_IS_RELATIVE) return { (int) ((float) size.x * (float) extent.x), (int) ((float) size.y * (float) extent.y) };
  
  return (kb_int2) { (int) size.x, (int) size.y };
}

KB_API void kb_graphics_init(const kb_graphics_init_info info) {
  kb_platform_graphics_init(info);

  for (uint32_t pass_i = 0; pass_i < KB_CONFIG_MAX_RENDERPASSES; ++pass_i) {
    draw_call_cache_pos[pass_i] = 0;
    draw_call_cache[pass_i]     = KB_DEFAULT_ALLOC_TYPE(kb_render_call, KB_CONFIG_MAX_DRAW_CALLS);
    
    compute_call_cache_pos[pass_i] = 0;
    compute_call_cache[pass_i]     = KB_DEFAULT_ALLOC_TYPE(kb_compute_call, KB_CONFIG_MAX_DRAW_CALLS);
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

    attachment = {
      .flags  = attachment_info.flags,
      .size   = attachment_info.size,
    };

    if (!(attachment.flags & KB_ATTACHMENT_FLAGS_SURFACE_PROXY)) {
      // Not surface proxy, create attachment texture
//      KB_ASSERT(attachment_info.usage & KB_TEXTURE_USAGE_RENDER_TARGET, "Graphics pipeline attachments must be render targets");

      kb_int2 attachment_size = calculate_attachment_size(current_extent, attachment_info.size, attachment_info.flags);

      attachment.format = attachment_info.format;
      attachment.usage = attachment_info.usage;
      attachment.texture = kb_texture_create({
        .rwops    = NULL,
        .usage    = attachment_info.usage,
        .texture  = {
          .format = attachment_info.format,
          .width  = (uint32_t) attachment_size.x,
          .height = (uint32_t) attachment_size.y,
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
    KB_DEFAULT_FREE(compute_call_cache[pass_i]);
  }
  
  destruct_encoder_pools();
}

KB_API void kb_graphics_run_encoders() {
  // Fill caches
  for (uint32_t encoder_i = 0; encoder_i < current_encoder_pool().count; ++encoder_i) {
    kb_encoder_state& state = current_encoder_pool().states[encoder_i];

    for (uint32_t call_i = 0; call_i < state.draw_call_count; ++call_i) {
      kb_render_call& call = state.draw_calls[call_i];
      KB_ASSERT_VALID(call.pipeline);
      
      uint32_t pass = pipeline_info_ref(call.pipeline)->pass;
      draw_call_cache[pass][draw_call_cache_pos[pass]++] = call;
    }

    for (uint32_t call_i = 0; call_i < state.compute_call_count; ++call_i) {
      kb_compute_call& call = state.compute_calls[call_i];
      KB_ASSERT_VALID(call.pipeline);

      uint32_t pass = pipeline_info_ref(call.pipeline)->pass;
      compute_call_cache[pass][compute_call_cache_pos[pass]++] = call;
    }
  }
    
  // Run passes
  for (uint32_t pass_i = 0; pass_i < KB_CONFIG_MAX_RENDERPASSES; ++pass_i) {
    if (draw_call_cache_pos[pass_i] > 0) {
      // Sort draw calls
      kb_sort(draw_call_cache[pass_i], draw_call_cache_pos[pass_i], sizeof(kb_render_call), draw_call_compare);

      // Submit render calls
      kb_platform_graphics_submit_render_pass(pass_i, draw_call_cache[pass_i], draw_call_cache_pos[pass_i]);
    }
    
    if (compute_call_cache_pos[pass_i] > 0) {
      // Submit compute calls
      kb_platform_graphics_submit_compute_pass(pass_i, compute_call_cache[pass_i], compute_call_cache_pos[pass_i]);
    }
    
    compute_call_cache_pos[pass_i] = 0;
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

      kb_pipe_attachment& attachment = graphics_pipe->attachments[attachment_i];

      if (attachment.flags & (KB_ATTACHMENT_FLAGS_USE_SURFACE_SIZE | KB_ATTACHMENT_FLAGS_SIZE_IS_RELATIVE)) {
        kb_int2 attachment_size = calculate_attachment_size(current_extent, attachment.size, attachment.flags);

        kb_platform_graphics_texture_destruct(texture);
        
        kb_platform_graphics_texture_construct(texture, {
          .usage  = attachment.usage,
          .texture = {
            .width  = (uint32_t) attachment_size.x,
            .height = (uint32_t) attachment_size.y,
            .format = attachment.format,
          }
        });
      }
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

KB_API kb_pass_info* kb_graphics_get_pass_info(uint32_t pass) {
  KB_ASSERT_NOT_NULL(graphics_pipe);
  KB_ASSERT(graphics_pipe->pass_count > pass, "Invalid Pass");

  return &graphics_pipe->passes[pass];
}

KB_API bool kb_graphics_pipe_attachment_surface_proxy(uint32_t attachment) {
  KB_ASSERT_NOT_NULL(graphics_pipe);
  KB_ASSERT(graphics_pipe->attachment_count > attachment, "Invalid Attachment");

  return graphics_pipe->attachments[attachment].flags & KB_ATTACHMENT_FLAGS_SURFACE_PROXY;
}

KB_API void kb_graphics_pipe_destruct(kb_graphics_pipe handle) {
  KB_ASSERT_VALID(handle);

  kb_log_debug("kb_graphics_pipe_destruct");
}

KB_API void kb_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {
  KB_ASSERT_VALID(handle);
  
  pipeline_info_ref(handle)->uniform_layout = info.uniform_layout;
  pipeline_info_ref(handle)->pass           = info.pass;

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
      if (layout->vertex_uniforms[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_VERTEX);
        out.type = KB_BINDING_TYPE_UNIFORM_BUFFER;
        out.vertex_slot = layout->vertex_uniforms[i].slot;
      }
      
      if (layout->fragment_uniforms[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_FRAGMENT);
        out.type = KB_BINDING_TYPE_UNIFORM_BUFFER;
        out.fragment_slot = layout->fragment_uniforms[i].slot;
      }
      
      if (layout->compute_uniforms[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_COMPUTE);
        out.type = KB_BINDING_TYPE_UNIFORM_BUFFER;
        out.fragment_slot = layout->compute_uniforms[i].slot;
      }
    }
  }

  if (type == KB_BINDING_TYPE_TEXTURE) {
    for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
      if (layout->vertex_textures[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_VERTEX);
        out.type = KB_BINDING_TYPE_TEXTURE;
        out.vertex_slot = layout->vertex_textures[i].slot;
      }
      
      if (layout->fragment_textures[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_FRAGMENT);
        out.type = KB_BINDING_TYPE_TEXTURE;
        out.fragment_slot = layout->fragment_textures[i].slot;
      }
      
      if (layout->compute_textures[i].hash == hash) {
        out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_COMPUTE);
        out.type = KB_BINDING_TYPE_TEXTURE;
        out.fragment_slot = layout->compute_textures[i].slot;
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

KB_API void kb_encoder_bind_pipeline(kb_encoder encoder, kb_pipeline pipeline) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(pipeline);

  current_encoder_frame(encoder).pipeline = pipeline;
}

KB_API void kb_encoder_bind_vertex_buffer(kb_encoder encoder, uint32_t slot, kb_buffer_memory memory) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(memory.buffer);

  current_encoder_frame(encoder).vertex_buffer_bindings[slot].memory = memory;
}

KB_API void kb_encoder_bind_index_buffer(kb_encoder encoder, kb_index_type type, kb_buffer_memory memory) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(memory.buffer);

  current_encoder_frame(encoder).index_buffer.memory      = memory;
  current_encoder_frame(encoder).index_buffer.index_type  = type;
}

KB_API void kb_encoder_bind_texture(kb_encoder encoder, const kb_uniform_slot slot, kb_texture texture) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(texture);

  KB_ASSERT(slot.vertex_slot    < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Vertex slot index too large");
  KB_ASSERT(slot.fragment_slot  < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Fragment slot index too large");
  KB_ASSERT(slot.compute_slot   < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Compute slot index too large");

  if (slot.stage & KB_SHADER_STAGE_VERTEX) {
    kb_texture_binding& binding = current_encoder_frame(current_encoder_state(encoder)).vertex_texture_bindings[slot.vertex_slot];
    binding.texture  = texture;
  }
  
  if (slot.stage & KB_SHADER_STAGE_FRAGMENT) {
    kb_texture_binding& binding = current_encoder_frame(current_encoder_state(encoder)).fragment_texture_bindings[slot.fragment_slot]; 
    binding.texture  = texture;
  }

  if (slot.stage & KB_SHADER_STAGE_COMPUTE) {
    kb_texture_binding& binding = current_encoder_frame(current_encoder_state(encoder)).compute_texture_bindings[slot.compute_slot]; 
    binding.texture  = texture;
  }
}

KB_API void kb_encoder_bind_uniform(kb_encoder encoder, const kb_uniform_slot slot, kb_buffer_memory memory) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(memory.buffer);

  if (slot.stage & KB_SHADER_STAGE_VERTEX) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).vertex_uniform_bindings[slot.vertex_slot];
    binding->memory = memory;
  }
  
  if (slot.stage & KB_SHADER_STAGE_FRAGMENT) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).fragment_uniform_bindings[slot.fragment_slot]; 
    binding->memory = memory;
  }
  
  if (slot.stage & KB_SHADER_STAGE_COMPUTE) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).compute_uniform_bindings[slot.compute_slot]; 
    binding->memory = memory;
  }
}

KB_API void kb_encoder_submit_compute(kb_encoder encoder, kb_int3 group_size, kb_int3 groups) {
  KB_ASSERT_VALID(encoder);
  
  kb_encoder_state& state = current_encoder_state(encoder);
  kb_encoder_frame& frame = current_encoder_frame(encoder);

  KB_ASSERT_VALID(frame.pipeline);

  kb_compute_call& call = state.compute_calls[state.compute_call_count++];

  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++binding) {
    call.uniform_bindings[binding]  = frame.compute_uniform_bindings[binding];
    call.texture_bindings[binding]  = frame.compute_texture_bindings[binding];
  }
  
  call.pipeline   = frame.pipeline;
  call.group_size = group_size;
  call.groups     = groups;
}

KB_API void kb_encoder_submit_draw(kb_encoder encoder, uint32_t first_index, uint32_t first_vertex, uint32_t index_count, uint32_t instance_count) {
  KB_ASSERT_VALID(encoder);

  kb_encoder_state& state = current_encoder_state(encoder);
  kb_encoder_frame& frame = current_encoder_frame(encoder);

  KB_ASSERT_VALID(frame.pipeline);

  KB_ASSERT(state.draw_call_count < KB_CONFIG_MAX_DRAW_CALLS, "Too many draw calls (KB_CONFIG_MAX_DRAW_CALLS)");
  
  kb_render_call& call = state.draw_calls[state.draw_call_count++];
  
  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++binding) {
    call.vertex_uniform_bindings[binding] = frame.vertex_uniform_bindings[binding];
    call.vertex_texture_bindings[binding] = frame.vertex_texture_bindings[binding];
    call.fragment_uniform_bindings[binding] = frame.fragment_uniform_bindings[binding];
    call.fragment_texture_bindings[binding] = frame.fragment_texture_bindings[binding];
  }

  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS; ++binding) {
    call.vertex_buffer_bindings[binding] = frame.vertex_buffer_bindings[binding];
  }

  call.pipeline        = frame.pipeline;
  call.index_buffer    = frame.index_buffer;
  call.first_vertex    = first_vertex;
  call.first_index     = first_index;
  call.index_count     = index_count;
  call.instance_count  = instance_count;
}

KB_API void* kb_graphics_get_buffer_mapped(kb_buffer_memory memory) {
  KB_ASSERT_VALID(memory.buffer);

  return kb_platform_graphics_buffer_mapped(memory);
}

KB_API kb_int2 kb_graphics_get_extent() {    
  return current_extent;
}

KB_API float kb_graphics_get_aspect() {
  kb_int2 size = kb_graphics_get_extent();
  return float(size.x) / float(size.y);
}
