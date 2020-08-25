#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "kb/core.h"

KB_HANDLE(FileHandle);

void open_filesystem(const char* root);

bool file_exists(const char* path);

FileHandle get_file_handle(const char* path);

#ifdef __cplusplus
}
#endif
