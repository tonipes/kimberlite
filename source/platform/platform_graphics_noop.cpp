// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>
#include <kb/log.h>

uint8_t scratch[KB_CONFIG_TRANSIENT_BUFFER_SIZE];

KB_API void kb_platform_pump_events() {
  
}

KB_API uint32_t kb_graphics_get_current_resource_slot() {
  return 0;
}

KB_API bool kb_platform_should_close() {
  return false;
}

KB_API kb_rwops* kb_platform_open_file_rwops(const char* path) {
  return nullptr;
}

KB_API void kb_platform_close_file_rwops(kb_rwops* rwops) {
  
}

KB_API void kb_platform_graphics_init(const kb_graphics_init_info info) {

}

KB_API void kb_platform_graphics_deinit() {
  
}

KB_API void kb_platform_graphics_frame() {
  
}

KB_API Int2 kb_platform_graphics_get_extent() {
  return { 1, 1 };
}

KB_API void kb_platform_graphics_wait_device_idle() {
  
}

KB_API void kb_platform_graphics_submit_calls(kb_renderpass pass, kb_graphics_call* calls, uint32_t call_count) {
  
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

KB_API uint64_t kb_platform_graphics_transient_used() {
  return 0;
}

KB_API uint64_t kb_platform_graphics_transient_capacity() {
  return KB_CONFIG_TRANSIENT_BUFFER_SIZE;
}

KB_API void* kb_platform_graphics_buffer_mapped(kb_buffer buffer) {
  return nullptr;
}

KB_API void kb_platform_buffer_construct(kb_buffer handle, const kb_buffer_create_info info) {
  
}

KB_API void kb_platform_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {
  
}

KB_API void kb_platform_texture_construct(kb_texture handle, const kb_texture_create_info info) {
  
}

KB_API void kb_platform_renderpass_construct(kb_renderpass handle, const kb_renderpass_create_info info) {
  
}

KB_API void kb_platform_buffer_destruct(kb_buffer handle) {
  
}

KB_API void kb_platform_pipeline_destruct(kb_pipeline handle) {
  
}

KB_API void kb_platform_texture_destruct(kb_texture handle) {
  
}

KB_API void kb_platform_renderpass_destruct(kb_renderpass handle) {
  
}

KB_API Int2 kb_platform_surface_get_size() {
  return { 1, 1 };
}

KB_API float kb_platform_surface_get_aspect() {
  return 1.0f;
}

KB_API void* kb_platform_surface_get_native_window_handle() {
  return nullptr;
}

KB_API void kb_platform_surface_create(uint32_t w, uint32_t h) {
  
}

KB_API void kb_platform_surface_set_system_cursor_visible   (bool visible) {
  
}

KB_API float kb_platform_surface_get_scaling() {
  return 1.0f;
}

KB_API void kb_platform_surface_destroy() {
  
}
