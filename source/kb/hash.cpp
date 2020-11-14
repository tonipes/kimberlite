// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/hash.h>

#include <kb/crt.h>

// Murmur Hash

inline void scramble(uint32_t& h, uint32_t& k) {
  k *= 0xcc9e2d51; 
  k ^= (k << 15) | (k >> 17);
  k *= 0x1b873593; 
  h ^= k;
}

void mix_tail(kb_hash_gen* gen, const uint8_t* d, int len) {
  KB_ASSERT_NOT_NULL(gen);

  while (len && ((len<4) || gen->count)) {
    gen->k |= (*d++) << (gen->count * 8);

    gen->count++;
    len--;

    if (gen->count == 4) {
      scramble(gen->h, gen->k);
      gen->k = 0;
      gen->count = 0;
    }
  }
}

void kb_hash_begin(kb_hash_gen* gen) {
  gen->h      = 0;
  gen->k      = 0;
  gen->count  = 0;
  gen->size   = 0;
}

void kb_hash_add(kb_hash_gen* gen, const void* data, int len) {
  KB_ASSERT_NOT_NULL(gen);
  KB_ASSERT_NOT_NULL(data);

  const uint8_t* d = (const uint8_t*) data;
  gen->size += len;

  mix_tail(gen, d, len);

  while(len >= 4) {
    uint32_t kk = *(uint32_t*)d;

    scramble(gen->h, kk);

    d += 4;
    len -= 4;
  }

  mix_tail(gen, d, len);
}

kb_hash kb_hash_end(kb_hash_gen* gen) {
  KB_ASSERT_NOT_NULL(gen);

  scramble(gen->h, gen->k);
  scramble(gen->h, gen->size);

  gen->h ^= gen->h >> 13;
  gen->h *= 0xc2b2ae35;
  gen->h ^= gen->h >> 15;
  
  return gen->h;
}

kb_hash kb_hash_string(const char* str) {
  if (str == NULL) { return 0; }

  uint32_t len = kb_strlen(str);
  kb_hash_gen gen;
  kb_hash_begin(&gen);
  kb_hash_add(&gen, str, len);

  return kb_hash_end(&gen);
}
