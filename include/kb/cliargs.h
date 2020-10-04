#pragma once

#include <kb/core.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int32_t       argc;
  const char**  argv;
} kb_cli_args;

KB_API void kb_cliargs_init       (kb_cli_args* args, int32_t argc, const char** argv);
KB_API bool kb_cliargs_get_str    (kb_cli_args* args, const char**  value, const char* key);
KB_API bool kb_cliargs_get_int    (kb_cli_args* args, int32_t* value, const char* key);
KB_API bool kb_cliargs_get_float  (kb_cli_args* args, float* value, const char* key);
KB_API bool kb_cliargs_get_bool   (kb_cli_args* args, bool* value, const char* key);

#ifdef __cplusplus
}
#endif
