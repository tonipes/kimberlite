// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_log_level {
  KB_LOG_LEVEL_UNKNOWN  = 0,
  KB_LOG_LEVEL_TRACE    = 1,
  KB_LOG_LEVEL_DEBUG    = 2,
  KB_LOG_LEVEL_INFO     = 3,
  KB_LOG_LEVEL_WARN     = 4,
  KB_LOG_LEVEL_ERROR    = 5,
  KB_LOG_LEVEL_FATAL    = 6,
} kb_log_level;

void kb_log_line        (kb_log_level level, const char* msg);
void kb_log_set_level   (kb_log_level level);
void kb_log_set_color   (bool enabled);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <fmt/format.h>

template <typename... T>
inline auto kb_log_trace(const T&... args) -> void {
  kb_log_line(KB_LOG_LEVEL_TRACE, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_debug(const T&... args) -> void { 
  kb_log_line(KB_LOG_LEVEL_DEBUG, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_info(const T&... args) -> void  { 
  kb_log_line(KB_LOG_LEVEL_INFO, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_warn(const T&... args) -> void  { 
  kb_log_line(KB_LOG_LEVEL_WARN, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_error(const T&... args) -> void { 
  kb_log_line(KB_LOG_LEVEL_ERROR, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_fatal(const T&... args) -> void { 
  kb_log_line(KB_LOG_LEVEL_FATAL, fmt::format(args...).c_str());
};

#endif
