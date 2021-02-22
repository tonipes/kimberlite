// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/algo.h>

KB_API uint32_t kb_alg_select_unique(uint32_t* src, uint32_t* dst, size_t count) {
  uint32_t p = 0;
  for (uint32_t c = 0; c < count; c++) {
    bool found = false;
    for (uint32_t i = 0; i < p; i++) {
      if (dst[i] == src[c]) { found = true; break; }
    }
    if (!found) { dst[p++] = src[c]; }
  }
  return p;
}
