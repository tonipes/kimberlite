// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_build_info {
  const char* cpu_str;
  const char* arch_str;
  const char* platform_str;
  const char* build_str;
  const char* version_str;
  const char* framework_str;
  const char* compiler_str;
  const char* cppver_str;
  const char* id_str;
} kb_build_info;

KB_API const kb_build_info*  kb_build_get_info ();

#ifdef __cplusplus
}
#endif
