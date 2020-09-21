#pragma once

#include <kb/variant.h>
#include <kb/rwops.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API bool kb_yaml_parse (Variant* variant, RWops* rwops);

#ifdef __cplusplus
}
#endif
