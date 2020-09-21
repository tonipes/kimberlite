#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KB_LOG_LEVEL_TRACE  = 1,
  KB_LOG_LEVEL_DEBUG  = 2,
  KB_LOG_LEVEL_INFO   = 3,
  KB_LOG_LEVEL_WARN   = 4,
  KB_LOG_LEVEL_ERROR  = 5,
  KB_LOG_LEVEL_FATAL  = 6,
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
