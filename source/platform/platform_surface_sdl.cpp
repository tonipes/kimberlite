// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

static SDL_Window* window_ptr;
static bool should_close = false;

static const auto sdl_window_flags = 0
  | SDL_WINDOW_SHOWN
  | SDL_WINDOW_RESIZABLE
  | SDL_WINDOW_VULKAN
  | SDL_WINDOW_ALLOW_HIGHDPI
;


static int event_watch(void* userdata, SDL_Event* event) {
  if (event->type == SDL_WINDOWEVENT) {
    if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
      // kb_log_debug("Window resized");      
    }
    if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      // kb_log_debug("Window size changed");      
    }
  }

  return 1;
}

KB_API void kb_platform_surface_set_system_cursor_visible(bool visible) {
  SDL_ShowCursor(visible);
}

KB_API void kb_platform_surface_create(uint32_t w, uint32_t h) {
  window_ptr = SDL_CreateWindow(
    "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, sdl_window_flags
  );

  SDL_AddEventWatch(event_watch, nullptr);
}

KB_API void kb_platform_surface_destroy() {
  SDL_DestroyWindow(window_ptr);
}

KB_API Int2 kb_platform_surface_get_size() {
  Int2 size;
  SDL_GetWindowSize(window_ptr, &size.x, &size.y);
  return size;
}

KB_API float kb_platform_surface_get_aspect() {
  Int2 size = kb_platform_surface_get_size();
  return float(size.x) / float(size.y);
}

KB_API float kb_platform_surface_get_scaling() {
  float ddpi;
  float hdpi;
  float vdpi;

  int display = SDL_GetWindowDisplayIndex(window_ptr);

  if (SDL_GetDisplayDPI(display, &ddpi, &hdpi, &vdpi) != 0) {
    return 1.0f;
  }

  return hdpi / 72.0f; // TODO: Do not hardcode default dpi
  // return 2.0f;
}

KB_API void* kb_platform_surface_get_native_window_handle() {
  SDL_SysWMinfo wmi;
  
  SDL_VERSION(&wmi.version);

  if (!SDL_GetWindowWMInfo(window_ptr, &wmi)) {
    kb_log_error("Couldn't get syswminfo");
  }

#if KB_PLATFORM_LINUX
  kb_log_trace("Linux window handle");
  return (void*)wmi.info.x11.window;
#elif KB_PLATFORM_MACOS
  kb_log_trace("macos window handle");
  return (void*)wmi.info.cocoa.window;
#elif KB_PLATFORM_WINDOWS
  kb_log_trace("windows window handle");
  return (void*)wmi.info.win.window;
#endif
  return nullptr;
}

KB_API bool kb_platform_should_close() {
  return should_close;
}

KB_API void kb_platform_pump_events() {
  SDL_Event e;
  
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      should_close = true;
    }
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
      should_close = true;
    }
  }
}
