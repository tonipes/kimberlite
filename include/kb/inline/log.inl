#pragma once

#include "fmt/format.h"

namespace Log {
  template <typename... T>
  inline auto trace(const T&... args) -> void {
    log_line(KBLogTrace, fmt::format(args...).c_str());
  };

  template <typename... T>
  inline auto debug(const T&... args) -> void { 
    log_line(KBLogDebug, fmt::format(args...).c_str());
  };

  template <typename... T>
  inline auto info(const T&... args) -> void  { 
    log_line(KBLogInfo, fmt::format(args...).c_str());
  };

  template <typename... T>
  inline auto warn(const T&... args) -> void  { 
    log_line(KBLogWarn, fmt::format(args...).c_str());
  };

  template <typename... T>
  inline auto error(const T&... args) -> void { 
    log_line(KBLogError, fmt::format(args...).c_str());
  };

  template <typename... T>
  inline auto fatal(const T&... args) -> void { 
    log_line(KBLogFatal, fmt::format(args...).c_str());
  };
};
