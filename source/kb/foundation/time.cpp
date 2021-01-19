// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/time.h>

#include <sys/time.h>

#define TIME_FREQ 1000000LL

const int64_t kb_time_ref = kb_time_get_raw();

int64_t kb_time_get_raw() {
  struct timeval now;
  gettimeofday(&now, 0);
  return now.tv_sec * TIME_FREQ + now.tv_usec;
}

int64_t kb_current_time() {
  return kb_time_get_raw() - kb_time_ref;
}

int64_t kb_time_get_frequency() {
  return TIME_FREQ;
}

float kb_time() {
  return double(kb_current_time()) / double(kb_time_get_frequency());
}
