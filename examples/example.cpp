// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

// #include <kb/graphics.h>
#include <kb/input.h>
#include <kb/log.h>
#include <kb/time.h>
#include <kb/platform.h>
// #include <kb/geometry.h>
// #include <kb/font.h>
// #include <kb/vertex.h>
// #include <kb/gizmo.h>
// #include <kb/hash.h>

const bool      vsync             = true;
const bool      hide_cursor       = false;
const uint32_t  msaa              = 0;
const Int2      resolution        = { 1600, 900 };
const float     max_frametime     = 0.33f;

int main(int argc, char** argv) {
  kb_log_set_level(KB_LOG_LEVEL_TRACE);

  kb_graphics_init_info graphics_init_info {};
  graphics_init_info.msaa         = msaa;
  graphics_init_info.vsync        = vsync;
  graphics_init_info.resolution   = resolution;
  graphics_init_info.hide_cursor  = hide_cursor;

  kb_graphics_init(graphics_init_info);

  kb_input_init_info input_init_info {};
  kb_input_init(input_init_info);

  double t = 0.0;
  double dt = 0.016;
  double current_time = kb_time();
  double accumulator = 0.0;

  while (!kb_platform_should_close()) {
    kb_platform_pump_events();

    double new_time = kb_time();
    double frame_time = new_time - current_time;
    if (frame_time > max_frametime) frame_time = max_frametime;
    current_time = new_time;

    accumulator += frame_time;
    
    while (accumulator >= dt) {
      t += dt;
      accumulator -= dt;        
      kb_input_frame();
      
      // Update
    }

    // Render
    
    kb_graphics_frame();
  }

  kb_graphics_wait_device_idle();
  
  kb_input_deinit();
  kb_graphics_deinit();

  return 0;
}
