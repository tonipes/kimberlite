#pragma once

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KB_ASSERT
  #define KB_ASSERT(e, m) assert(e && m)
#endif

#ifdef __cplusplus
}
#endif
