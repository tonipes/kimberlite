#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// #include "types.h"

typedef enum {
  KBLogTrace  = 1,
  KBLogDebug  = 2,
  KBLogInfo   = 3,
  KBLogWarn   = 4,
  KBLogError  = 5,
  KBLogFatal  = 6,
} LogLevel;

void log_line(LogLevel level, const char* msg);
void set_log_level(LogLevel level);

#ifdef __cplusplus
}
#endif
