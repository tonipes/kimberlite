#pragma once

#include <kb/api.h>
#include <kb/types.h>
#include <kb/handle.h>
#include <kb/rwops.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_HANDLE(TextureHandle);

typedef enum {
  KB_FORMAT_UNSUPPORTED  = 0,
  KB_FORMAT_R8           = 1,
  KB_FORMAT_R8G8         = 2,
  KB_FORMAT_R8G8B8       = 3,
  KB_FORMAT_R8G8B8A8     = 4,
} Format;

typedef enum {
  KB_FILTER_NEAREST = 0,
  KB_FILTER_LINEAR  = 1,
} Filter;

typedef struct {
  Filter filter;
} SamplerInfo;

typedef struct {
  uint32_t  width;
  uint32_t  height;
  Format    format;
  bool      create_mipmaps;
} TextureInfo;

typedef struct {
  TextureInfo texture;
  SamplerInfo sampler;
  uint64_t    pixel_data_size;
  void*       pixel_data;
} Texture;

KB_API void kb_texture_read(Texture* dst, RWops* src);

KB_API void kb_texture_write(const Texture* src, RWops* dst);

#ifdef __cplusplus
}
#endif
