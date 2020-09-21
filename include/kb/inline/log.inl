#pragma once

#include "fmt/format.h"

template <typename... T>
inline auto kb_log_trace(const T&... args) -> void {
  kb_log_line(KBLogTrace, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_debug(const T&... args) -> void { 
  kb_log_line(KBLogDebug, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_info(const T&... args) -> void  { 
  kb_log_line(KBLogInfo, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_warn(const T&... args) -> void  { 
  kb_log_line(KBLogWarn, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_error(const T&... args) -> void { 
  kb_log_line(KBLogError, fmt::format(args...).c_str());
};

template <typename... T>
inline auto kb_log_fatal(const T&... args) -> void { 
  kb_log_line(KBLogFatal, fmt::format(args...).c_str());
};
