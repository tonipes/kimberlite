#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef uint16_t Handle;

static const Handle InvalidHandle = UINT16_MAX;

static inline bool is_valid_handle(const Handle handle) { return handle != InvalidHandle; }

#define KB_HANDLE(name)                                                                           \
	typedef struct {                                                                                \
    uint16_t idx;                                                                                 \
  } name;                                                                                         \
  static inline bool is_valid##name(const name handle) { return handle.idx != InvalidHandle; }    \
  static const name Invalid##name = { InvalidHandle };                                            \

#define KB_INVALID_HANDLE { InvalidHandle }

#define INVALID_HANDLE_CHECK(name, handle) if (!is_valid_handle(handle.idx)) Log::error("Using invalid {} handle", #name);

#ifdef __cplusplus
}
#endif
