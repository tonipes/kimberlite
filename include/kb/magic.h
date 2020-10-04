#pragma once

#include <kb/rwops.h>

#ifdef __cplusplus
extern "C" {
#endif

bool kb_magic_read(kb_rwops* rwops, uint32_t magic);

#ifdef __cplusplus
}
#endif
