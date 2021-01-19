// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/file.h>

#include <kb/platform.h>

KB_API kb_stream* kb_stream_open_file(const char* path, kb_file_mode mode) {
  kb_stream* stream = KB_DEFAULT_ALLOC_TYPE(kb_stream, 1);
  int res = kb_platform_stream_file_open(stream, path, mode);
  
  if (res != 0) {
    KB_DEFAULT_FREE(stream);
    return 0;
  }

  return stream;
}
