// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>

#ifdef __cplusplus

#include <fmt/format.h>

template <typename... T>
KB_API uint64_t kb_strfmt(char* out, int32_t size, const T&... args) {
  auto str = fmt::format(args...);
  int32_t len = str.length() < size ? str.length() : size;
  kb_strncpy(out, str.c_str(), len);
  return len;
}

#endif
