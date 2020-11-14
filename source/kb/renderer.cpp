// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/renderer.h>

#include <kb/platform.h>
#include <kb/time.h>
#include <kb/sampler.h>
#include <kb/vertex.h>

struct pipeline_info {
  kb_uniform_layout     uniform_layout;
  kb_vertex_layout_info vertex_layout;
};

kb_encoder_pool encoder_pools[KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
kb_sampler      frametime_sampler = kb_sampler_construct<100>();
double          last_frame_start_time = 0;
kb_frame_stats  frame_stats;
kb_frame_stats  frame_stats_curr;

static inline kb_encoder_pool&   current_encoder_pool   ()                          { return encoder_pools[kb_graphics_get_current_resource_slot()]; }
static inline kb_encoder_state&  current_encoder_state  (kb_encoder encoder)        { return current_encoder_pool().states[kb_to_arr(encoder)]; }
static inline kb_encoder_frame&  current_encoder_frame  (kb_encoder_state& state)   { return state.stack[state.stack_pos]; }
static inline kb_encoder_frame&  current_encoder_frame  (kb_encoder encoder)        { return current_encoder_frame(current_encoder_state(encoder)); }

KB_RESOURCE_ALLOC_FUNC_DEF  (buffer,        kb_buffer,        kb_buffer_create_info,        KB_CONFIG_MAX_BUFFERS);
KB_RESOURCE_ALLOC_FUNC_DEF  (texture,       kb_texture,       kb_texture_create_info,       KB_CONFIG_MAX_TEXTURES);
KB_RESOURCE_ALLOC_FUNC_DEF  (pipeline,      kb_pipeline,      kb_pipeline_create_info,      KB_CONFIG_MAX_PROGRAMS);

KB_RESOURCE_DATA_HASHED_DEF (buffer,        kb_buffer);
KB_RESOURCE_DATA_HASHED_DEF (texture,       kb_texture);
KB_RESOURCE_DATA_HASHED_DEF (pipeline,      kb_pipeline);

KB_RESOURCE_STORAGE_DEF     (pipeline_info,         kb_pipeline,      pipeline_info,    KB_CONFIG_MAX_PROGRAMS);

static inline void reset_encoder_pool(kb_encoder_pool* pool) {  
  for (uint32_t i = 0; i < pool->count; ++i) {
    pool->states[i] = {};
  }

  pool->count = 0;
}

KB_INTERNAL void update_stats() {
  frame_stats = frame_stats_curr;
  frame_stats_curr = {};
  
  frame_stats_curr.draw_calls = 0;
  for (uint32_t i = 0; i < current_encoder_pool().count; ++i) {
    frame_stats_curr.draw_calls += current_encoder_pool().states[i].draw_call_count;
  }
  frame_stats_curr.buffer_count           = kb_buffer_count();
  frame_stats_curr.encoder_count         = current_encoder_pool().count;
  frame_stats_curr.pipeline_count        = kb_pipeline_count();
  frame_stats_curr.texture_count         = kb_texture_count();
  frame_stats_curr.frametime_avg         = frametime_sampler.avg;
  frame_stats_curr.frametime_min         = frametime_sampler.min;
  frame_stats_curr.frametime_max         = frametime_sampler.max;
  frame_stats_curr.transient_memory_used = kb_platform_graphics_transient_used();
}

KB_API void* kb_graphics_transient_alloc(uint64_t size, uint64_t align) {
  return kb_platform_graphics_transient_alloc(size, align);
}

KB_API void* kb_graphics_transient_at(uint64_t offset) {
  return kb_platform_graphics_transient_at(offset);
}

KB_API uint64_t kb_graphics_transient_offset(void* ptr) {
  return kb_platform_graphics_transient_offset(ptr);
}

KB_API void kb_graphics_init(const kb_graphics_init_info info) {
  kb_platform_graphics_init(info);
}

KB_API void kb_graphics_deinit() {
  kb_platform_graphics_wait_device_idle();
  
  kb_pipeline_purge();
  kb_texture_purge();

  kb_platform_graphics_deinit();
}

KB_API void kb_graphics_run_encoders() {
  for (uint32_t i = 0; i < current_encoder_pool().count; ++i) {
    kb_encoder_state& state = current_encoder_pool().states[i];
    kb_platform_graphics_submit_calls(state.draw_calls, state.draw_call_count);
  }
}

KB_API void kb_graphics_frame() {
  double current = kb_time();
  double frametime = current - last_frame_start_time;
  last_frame_start_time = current;
  
  kb_sampler_push(&frametime_sampler, frametime);

  update_stats();
  
  kb_platform_graphics_frame();

  reset_encoder_pool(&current_encoder_pool());
}

KB_API void kb_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {
  KB_ASSERT_VALID(handle);
  
  // TODO: Make a proper copy
  pipeline_info_ref(handle).uniform_layout  = info.uniform_layout;
  pipeline_info_ref(handle).vertex_layout   = info.vertex_layout;

  kb_platform_pipeline_construct(handle, info);
}

KB_API void kb_pipeline_destruct(kb_pipeline handle) {
  KB_ASSERT_VALID(handle);

  kb_platform_pipeline_destruct(handle);
}

KB_API void kb_texture_construct(kb_texture handle, const kb_texture_create_info info) {
  KB_ASSERT_VALID(handle);

  kb_platform_texture_construct(handle, info);
}

KB_API void kb_texture_destruct(kb_texture handle) {
  KB_ASSERT_VALID(handle);

  kb_platform_texture_destruct(handle);
}

KB_API void kb_buffer_construct(kb_buffer handle, const kb_buffer_create_info info) {
  KB_ASSERT_VALID(handle);

  kb_platform_buffer_construct(handle, info);
}

KB_API void kb_buffer_destruct(kb_buffer handle) {
  KB_ASSERT_VALID(handle);

  kb_platform_buffer_destruct(handle);
}

KB_API void kb_graphics_get_frame_stats(kb_frame_stats* stats) {
  KB_ASSERT_NOT_NULL(stats);

  *stats = frame_stats;
}

KB_API void kb_graphics_wait_device_idle() {
  kb_platform_graphics_wait_device_idle();
}

KB_API kb_uniform_slot kb_pipeline_get_uniform_slot(kb_pipeline pipeline, kb_hash hash, kb_shader_binding_type type, kb_shader_stage stage) {
  KB_ASSERT_VALID(pipeline);

  kb_uniform_slot out {};

  if (stage & KB_SHADER_STAGE_VERTEX) {

    if (type == KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER) {
      for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
        kb_uniform_buffer_info& buffer_info = pipeline_info_ref(pipeline).uniform_layout.vert_ubos[i];
        if (kb_hash_string(buffer_info.name) == hash) { // VBO found
          out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_VERTEX);
          out.type = KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER;
          out.vert_index = buffer_info.slot;
          break;
        }
      }
    }

    else if (type == KB_SHADER_BINDING_TYPE_TEXTURE) {
      for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
        kb_uniform_texture_info& texture_info = pipeline_info_ref(pipeline).uniform_layout.vert_textures[i];
        if (kb_hash_string(texture_info.name) == hash) { // Texture found
          out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_VERTEX);
          out.type = KB_SHADER_BINDING_TYPE_TEXTURE;
          out.vert_index = texture_info.slot;
          break;
        }
      }
    }

  }

  if (stage & KB_SHADER_STAGE_FRAGMENT) {

    if (type == KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER) {
      for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
        kb_uniform_buffer_info& buffer_info = pipeline_info_ref(pipeline).uniform_layout.frag_ubos[i];
        if (kb_hash_string(buffer_info.name) == hash) { // VBO found
          out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_FRAGMENT);
          out.type = KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER;
          out.frag_index = buffer_info.slot;
          break;
        }
      }
    }

    else if (type == KB_SHADER_BINDING_TYPE_TEXTURE) {
      for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
        kb_uniform_texture_info& texture_info = pipeline_info_ref(pipeline).uniform_layout.frag_textures[i];
        if (kb_hash_string(texture_info.name) == hash) { // Texture found
          out.stage = (kb_shader_stage) (out.stage | KB_SHADER_STAGE_FRAGMENT);
          out.type = KB_SHADER_BINDING_TYPE_TEXTURE;
          out.frag_index = texture_info.slot;
          break;
        }
      }
    }

  }
  
  return out;
}

KB_API kb_encoder kb_encoder_begin() {
  kb_encoder_pool& pool = current_encoder_pool();

  uint32_t slot = pool.count++;
  kb_encoder encoder = kb_encoder_from_arr(slot);
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

  current_encoder_frame(encoder).pipeline = pipeline;
}

KB_API void kb_encoder_bind_buffer(kb_encoder encoder, uint32_t slot, kb_buffer buffer, uint64_t offset) {
  // NOTE: Buffer can be invalid (transient)
  KB_ASSERT_VALID(encoder);

  current_encoder_frame(encoder).vertex_buffer_bindings[slot].is_set = true;;
  current_encoder_frame(encoder).vertex_buffer_bindings[slot].buffer = buffer;
  current_encoder_frame(encoder).vertex_buffer_bindings[slot].offset = offset;
}

KB_API void kb_encoder_bind_index_buffer(kb_encoder encoder, kb_buffer buffer, uint64_t offset, kb_index_type type) {
  // NOTE: Buffer can be invalid (transient)
  KB_ASSERT_VALID(encoder);

  current_encoder_frame(encoder).index_buffer.buffer      = buffer;
  current_encoder_frame(encoder).index_buffer.offset      = offset;
  current_encoder_frame(encoder).index_buffer.index_type  = type;
}

KB_API void kb_encoder_bind_texture(kb_encoder encoder, const kb_uniform_slot slot, kb_texture texture) {
  KB_ASSERT_VALID(encoder);
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
  KB_ASSERT(slot.vert_index < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Slot index too large");
  KB_ASSERT(slot.frag_index < KB_CONFIG_MAX_UNIFORM_BINDINGS, "Slot index too large");

  uint64_t data_size = size;

  if (data_size == 0) return;
  KB_ASSERT_NOT_NULL(data);

  void* ptr = kb_graphics_transient_alloc(data_size, 256);
  
  if (!ptr) {
    kb_log_error("Failed to allocate transient data for uniform buffer binding");
    return;
  }

  kb_memcpy(ptr, data, data_size);
  
  if (slot.stage & KB_SHADER_STAGE_VERTEX) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).vert_uniform_bindings[slot.vert_index];
    binding->index  = slot.vert_index;
    binding->size   = data_size;
    binding->offset = kb_graphics_transient_offset(ptr);
  }
  
  if (slot.stage & KB_SHADER_STAGE_FRAGMENT) {
    kb_uniform_binding* binding = &current_encoder_frame(current_encoder_state(encoder)).frag_uniform_bindings[slot.frag_index]; 
    binding->index  = slot.frag_index;
    binding->size   = data_size;
    binding->offset = kb_graphics_transient_offset(ptr);
  }
}

KB_API void kb_encoder_submit(kb_encoder encoder, uint32_t first_index, uint32_t first_vertex, uint32_t index_count, uint32_t instance_count) {
  KB_ASSERT_VALID(encoder);

  if (index_count == 0) return;

  kb_encoder_state& state = current_encoder_state(encoder);
  kb_encoder_frame& frame = current_encoder_frame(encoder);
  
  KB_ASSERT_VALID(frame.pipeline);
  KB_ASSERT(state.draw_call_count < KB_CONFIG_MAX_DRAW_CALLS, "Too many draw calls (KB_CONFIG_MAX_DRAW_CALLS)");
  
  uint32_t call_idx = state.draw_call_count++;
  kb_graphics_call& call = state.draw_calls[call_idx];
  
  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++binding) {
    call.vert_uniform_bindings[binding] = frame.vert_uniform_bindings[binding];
    call.vert_texture_bindings[binding] = frame.vert_texture_bindings[binding];
    call.frag_uniform_bindings[binding] = frame.frag_uniform_bindings[binding];
    call.frag_texture_bindings[binding] = frame.frag_texture_bindings[binding];
  }

  for (uint32_t binding = 0; binding < KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS; ++binding) {
    call.vertex_buffer_bindings[binding] = frame.vertex_buffer_bindings[binding];
  }

  call.index_buffer         = frame.index_buffer;
  call.pipeline             = frame.pipeline;
  call.info.first_vertex    = first_vertex;
  call.info.first_index     = first_index;
  call.info.index_count     = index_count;
  call.info.instance_count  = instance_count;

}

KB_API void* kb_graphics_get_buffer_mapped(kb_buffer buffer) {
  KB_ASSERT_VALID(buffer);

  return kb_platform_graphics_buffer_mapped(buffer);
}
