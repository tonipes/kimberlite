// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/build.h>
#include <kb/foundation/core.h>

static const kb_build_info build_info {
  .cpu_str         = "" KB_PLATFORM_NAME,
  .arch_str        = "" KB_ARCH_NAME,
  .platform_str    = "" KB_PLATFORM_NAME,
  .build_str       = "" KB_BUILD_MODE_NAME,
  .version_str     = "" KB_VERSION_STR,
  .framework_str   = "" KB_NAME,
  .compiler_str    = "" KB_COMPILER_NAME,
  .cppver_str      = "" KB_CPP_NAME,
  .id_str          = KB_NAME " v" KB_VERSION_STR " / " KB_CPU_NAME " / " KB_ARCH_NAME " / " KB_PLATFORM_NAME " / " KB_BUILD_MODE_NAME " / " KB_COMPILER_NAME " / " KB_CPP_NAME,
};

KB_API const kb_build_info* kb_build_get_info() {
  return &build_info;
}
