#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

int64_t kb_time_get_raw();
int64_t kb_time_get_current();
int64_t kb_time_get_frequency();

// int64_t kb_time_get_raw();

float kb_time();



// int64_t kb_time();

// int64_t kb_time_get_frequency();



#ifdef __cplusplus
}
#endif
