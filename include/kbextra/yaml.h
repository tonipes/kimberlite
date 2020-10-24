// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>
#include <kb/rwops.h>

#include <kbextra/variant.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API bool kb_yaml_parse (kb_variant* variant, kb_rwops* rwops);

#ifdef __cplusplus
}
#endif
