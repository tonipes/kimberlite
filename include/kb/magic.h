#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <kb/types.h>
#include <kb/rwops.h>

bool kb_read_magic(RWops* rwops, uint32_t magic);

#ifdef __cplusplus
}
#endif
