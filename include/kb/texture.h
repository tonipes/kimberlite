#pragma once

#include <kb/core.h>

#include <kb/handle.h>
#include <kb/rwops.h>

KB_HANDLE(kb_texture);

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_format {
  KB_FORMAT_UNSUPPORTED  = 0,
  KB_FORMAT_R8           = 1,
  KB_FORMAT_R8G8         = 2,
  KB_FORMAT_R8G8B8       = 3,
  KB_FORMAT_R8G8B8A8     = 4,
} kb_format;

typedef enum kb_filter {
  KB_FILTER_NEAREST = 0,
  KB_FILTER_LINEAR  = 1,
} kb_filter;

typedef struct kb_sampler_info {
  kb_filter filter;
} kb_sampler_info;

typedef struct kb_texture_info {
  uint32_t  width;
  uint32_t  height;
  kb_format format;
  bool      create_mipmaps;
} kb_texture_info;

typedef struct kb_texture_data {
  kb_texture_info texture;
  kb_sampler_info sampler;
  uint64_t        pixel_data_size;
  void*           pixel_data;
} kb_texture_data;

KB_API void kb_texture_read(kb_texture_data* dst, kb_rwops* src);

KB_API void kb_texture_write(const kb_texture_data* src, kb_rwops* dst);

#ifdef __cplusplus
}
#endif
