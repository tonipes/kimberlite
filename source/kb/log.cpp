// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/log.h>

#include <kb/thread.h>
#include <kb/crt.h>

auto get_level_str(kb_log_level level) -> const char* {
  switch (level) {
    case KB_LOG_LEVEL_TRACE     : return "trace";
    case KB_LOG_LEVEL_DEBUG     : return "debug";
    case KB_LOG_LEVEL_INFO      : return "info ";
    case KB_LOG_LEVEL_WARN      : return "warn ";
    case KB_LOG_LEVEL_ERROR     : return "error";
    case KB_LOG_LEVEL_FATAL     : return "fatal";
    default                     : return "?????";
  }
}

auto get_level_color(kb_log_level level) -> const char* {
  switch (level) {
    case KB_LOG_LEVEL_TRACE     : return "0;37";
    case KB_LOG_LEVEL_DEBUG     : return "0;32";
    case KB_LOG_LEVEL_INFO      : return "0;34";
    case KB_LOG_LEVEL_WARN      : return "0;33";
    case KB_LOG_LEVEL_ERROR     : return "0;31";
    case KB_LOG_LEVEL_FATAL     : return "1;35";
    default                     : return "0;39";
  }
}

static kb_log_level log_level   = kb_log_level::KB_LOG_LEVEL_TRACE;
static bool         log_color   = false;
static bool         log_header  = false;
static kb_mutex*    log_mutex   = NULL; //kb_mutex_create();

inline void print(const char* str) {
  kb_printf("%s", str);
}

void kb_log_set_color(bool enabled) {
  log_color = enabled;
}

void kb_log_set_header(bool enabled) {
  log_header = enabled;
}

void kb_log_set_level(kb_log_level level) {
  log_level = level;
};

void kb_log_line(kb_log_level level, const char* msg) {
  if (level >= log_level) {
    if (log_mutex) kb_mutex_lock(log_mutex);
    
    {
      if (log_header) {
        if (log_color) {
          print("\e[");
          print(get_level_color(level));
          print("m");
        }
        
        print("[");
        print(get_level_str(level));
        print("] ");
        
        if (log_color) print("\e[0;37m");
        if (log_color) print("\e[0;39m");
      }

      print(msg);
      print("\n");
    }
    
    if (log_mutex) kb_mutex_unlock(log_mutex);
  }
}
