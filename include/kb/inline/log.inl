#pragma once

#include "fmt/format.h"

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
