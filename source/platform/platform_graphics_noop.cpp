// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

uint8_t scratch[KB_CONFIG_TRANSIENT_BUFFER_SIZE];

KB_API void kb_platform_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {

}

KB_API void kb_platform_pipeline_destruct(kb_pipeline handle) {

}

KB_API void kb_platform_texture_construct(kb_texture handle, const kb_texture_create_info info) {

}

KB_API uint32_t kb_graphics_get_current_resource_slot() {
  return 0;
}

KB_API void kb_platform_texture_destruct(kb_texture handle) {

}

KB_API void kb_platform_vertex_buffer_construct(kb_vertex_buffer handle, const kb_vertex_buffer_create_inf* info) {

}

KB_API void kb_platform_vertex_buffer_destruct(kb_vertex_buffer handle) { 

}

KB_API void kb_platform_index_buffer_construct(kb_index_buffer handle, const kb_index_buffer_create_info info) {

}

KB_API void kb_platform_index_buffer_destruct(kb_index_buffer handle) { 

}

KB_API void* kb_platform_graphics_transient_alloc(uint64_t size, uint64_t align) {
  return scratch;
}

KB_API void* kb_platform_graphics_transient_at(uint64_t offset) {
  return &scratch[offset];
}

KB_API uint64_t kb_platform_graphics_transient_offset(void* ptr) {
  return ((uint8_t*) ptr) - scratch;
}

KB_API void kb_platform_graphics_init(const kb_graphics_init_info info) {
  kb_platform_surface_create(info.resolution.x, info.resolution.y);
}

KB_API void kb_platform_graphics_wait_device_idle() {

}

KB_API void kb_platform_graphics_deinit() {  
  kb_platform_surface_destroy();
}

KB_API Int2 kb_graphics_get_extent() {
  return { 1, 1 };
}

KB_API void kb_platform_graphics_frame() {

}

KB_API void kb_platform_graphics_submit_calls(kb_graphics_call* calls, uint32_t call_count) {

}
