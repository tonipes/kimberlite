#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API void   kb_memset             (void* dst, uint8_t ch, size_t count);
KB_API void   kb_memcpy             (void* dst, const void* src, size_t count);
KB_API void   kb_memcpy_with_stride (void* dst, const void* src, size_t count, size_t index, size_t stride, size_t offset);
KB_API int    kb_strcmp             (const char* a, const char* b);
KB_API int    kb_strlen             (const char* a);
KB_API char*  kb_strcpy             (char* dst, const char* src);
KB_API int    kb_printf             (const char* format, ...);
KB_API int    kb_snprintf           (char* out, int32_t max, const char* format, ...);
KB_API int    kb_vsnprintf          (char* out, int32_t max, const char* format, va_list arg_list);
KB_API bool   kb_fromstr_bool       (bool*    dst, const char* str);
KB_API bool   kb_fromstr_int        (int32_t* dst, const char* str);
KB_API bool   kb_fromstr_float      (float*   dst, const char* str);
KB_API char*  kb_strdup             (const char* src);

#ifdef __cplusplus
}
#endif
