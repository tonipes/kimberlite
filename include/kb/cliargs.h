#pragma once

#include <kb/api.h>
#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int32_t argc;
  const char** argv;
} CliArgs;

KB_API void kb_cliargs_init       (CliArgs* args, int32_t argc, const char** argv);
KB_API bool kb_cliargs_get_str    (CliArgs* args, const char**  value, const char* key);
KB_API bool kb_cliargs_get_int    (CliArgs* args, int32_t* value, const char* key);
KB_API bool kb_cliargs_get_float  (CliArgs* args, float* value, const char* key);
KB_API bool kb_cliargs_get_bool   (CliArgs* args, bool* value, const char* key);

#ifdef __cplusplus
}
#endif
