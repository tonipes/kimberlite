#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef struct {
  int32_t argc;
  const char** argv;
} CliArgs;

void kb_cliargs_parse(CliArgs* args, int32_t argc, const char** argv);

bool kb_cliargs_get_str(CliArgs* args, const char**  value, const char* key);

// bool kb_cliargs_get_int(CliArgs* args, int32_t* value, const char* key);
// bool kb_cliargs_get_float(CliArgs* args, float* value, const char* key);
// bool kb_cliargs_get_bool(CliArgs* args, bool* value, const char* key);

#ifdef __cplusplus
}
#endif
