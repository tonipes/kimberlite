#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KBLogTrace  = 1,
  KBLogDebug  = 2,
  KBLogInfo   = 3,
  KBLogWarn   = 4,
  KBLogError  = 5,
  KBLogFatal  = 6,
} LogLevel;

void kb_log_line      (LogLevel level, const char* msg);
void kb_log_set_level (LogLevel level);

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

template <typename... T>
inline auto kb_log_debug(const T&... args) -> void;

template <typename... T>
inline auto kb_log_info(const T&... args) -> void;

template <typename... T>
inline auto kb_log_warn(const T&... args) -> void;

template <typename... T>
inline auto kb_log_error(const T&... args) -> void;

template <typename... T>
inline auto kb_log_fatal(const T&... args) -> void;

#include "inline/log.inl"

#endif
