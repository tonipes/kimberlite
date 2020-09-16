#pragma once 

#include <kb/rwops.h>

template <typename T> 
static uint64_t kb_write(RWops* rwops, const T& t) {
  return kb_rwops_write(rwops, &t, sizeof(T));
}

template <typename T> 
static uint64_t kb_read(RWops* rwops, T& t) {
  return kb_rwops_read(rwops, &t, sizeof(T));
}
