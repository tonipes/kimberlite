#pragma once

#include <kb/types.h>
#include <kb/rwops.h>

#ifdef __cplusplus
extern "C" {
#endif

bool kb_magic_read(RWops* rwops, uint32_t magic);

#ifdef __cplusplus
}
#endif
