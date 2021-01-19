// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kb_cli_args {
  int32_t       argc;
  const char**  argv;
} kb_cli_args;

KB_API void         kb_cliargs_init       (kb_cli_args* args, int32_t argc, const char** argv);
KB_API bool         kb_cliargs_has        (kb_cli_args* args, const char* key);
KB_API bool         kb_cliargs_get_str    (kb_cli_args* args, const char**  value, const char* key);
KB_API bool         kb_cliargs_get_int    (kb_cli_args* args, int32_t* value, const char* key);
KB_API bool         kb_cliargs_get_float  (kb_cli_args* args, float* value, const char* key);
KB_API bool         kb_cliargs_get_bool   (kb_cli_args* args, bool* value, const char* key);
KB_API const char*  kb_cliargs_find       (kb_cli_args* args, const char* key, int32_t skip);
KB_API uint32_t     kb_cliargs_count      (kb_cli_args* args, const char* key);

#ifdef __cplusplus
}
#endif
