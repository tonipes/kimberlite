#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#ifndef KB_ASSERT
  #define KB_ASSERT(e, m) assert(e && m)
#endif

#ifdef __cplusplus
}
#endif
