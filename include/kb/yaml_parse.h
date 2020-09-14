#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "variant.h"
#include "rwops.h"

KB_API bool kb_yaml_parse (Variant* variant, RWops* rwops);
// KB_API void kb_yaml_emit  (Variant* variant, RWops* rwops);

#ifdef __cplusplus
}
#endif
