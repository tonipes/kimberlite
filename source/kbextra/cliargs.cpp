// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/cliargs.h>

#include <kb/crt.h>

static const char* find(kb_cli_args* args, int32_t skip, const char* key, int32_t params) {
  KB_ASSERT_NOT_NULL(args);
  KB_ASSERT_NOT_NULL(key);

  for (int32_t ii = 0; ii < args->argc && 0 != kb_strcmp(args->argv[ii], "--"); ++ii) {
    const char* arg = args->argv[ii];
    if (*arg == '-') {
      ++arg;

      if (*arg == '-' && kb_strcmp(arg + 1, key) == 0) {
        if (skip == 0) {

          if (params == 0) {
            return "";
          }

          else if (ii + params < args->argc && '-' != *args->argv[ii + 1]) {
            return args->argv[ii + 1];
          }

          return NULL;
        }

        --skip;
        ii += params;
      }
    }
  }

  return NULL;
}

KB_API void kb_cliargs_init(kb_cli_args* args, int32_t argc, const char** argv) {
  KB_ASSERT_NOT_NULL(args);

  *args = {};

  args->argc = argc;
  args->argv = argv;
}

KB_API const char* kb_cliargs_find(kb_cli_args* args, const char* key, int32_t skip) {
  KB_ASSERT_NOT_NULL(args);

  return find(args, skip, key, 1);
}

KB_API bool kb_cliargs_has(kb_cli_args* args, const char* key) {
  KB_ASSERT_NOT_NULL(args);

  return find(args, 0, key, 0) != NULL;
}

KB_API bool kb_cliargs_get_str(kb_cli_args* args, const char** value, const char* key) {
  KB_ASSERT_NOT_NULL(args);
  KB_ASSERT_NOT_NULL(value);

  const char* arg = kb_cliargs_find(args, key, 0);
  *value = arg;
  return arg != NULL;
}

KB_API bool kb_cliargs_get_int(kb_cli_args* args, int32_t* value, const char* key) {
  KB_ASSERT_NOT_NULL(args);
  KB_ASSERT_NOT_NULL(value);

  const char* arg = kb_cliargs_find(args, key, 0);
  if (arg == NULL) return false;

  return kb_fromstr_int(value, arg);
}

KB_API bool kb_cliargs_get_float(kb_cli_args* args, float* value, const char* key) {
  KB_ASSERT_NOT_NULL(args);
  KB_ASSERT_NOT_NULL(value);

  const char* arg = kb_cliargs_find(args, key, 0);
  if (arg == NULL) return false;

  return kb_fromstr_float(value, arg);
}

KB_API bool kb_cliargs_get_bool(kb_cli_args* args, bool* value, const char* key) {
  KB_ASSERT_NOT_NULL(args);
  KB_ASSERT_NOT_NULL(value);

  const char* arg = kb_cliargs_find(args, key, 0);
  if (arg == NULL) return false;

  return kb_fromstr_bool(value, arg);
}

KB_API uint32_t kb_cliargs_count(kb_cli_args* args, const char* key) {
  KB_ASSERT_NOT_NULL(args);
  KB_ASSERT_NOT_NULL(key);
  
  uint32_t i = 0;
  while (true) {
    if (kb_cliargs_find(args, key, i) == NULL) {
      break;
    }
    ++i;
  }
  
  return i;
}
