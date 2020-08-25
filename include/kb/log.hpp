#pragma once

#include "log.h"

namespace Log {
  template <typename... T>
  inline auto debug(const T&... args) -> void;

  template <typename... T>
  inline auto info(const T&... args) -> void;

  template <typename... T>
  inline auto warn(const T&... args) -> void;

  template <typename... T>
  inline auto error(const T&... args) -> void;

  template <typename... T>
  inline auto fatal(const T&... args) -> void;
};

#include "inline/log.inl"
