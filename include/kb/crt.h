// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_signal {
  KB_SIGABRT  = 1,
  KB_SIGFPE   = 2,
  KB_SIGILL   = 3,
  KB_SIGINT   = 4,
  KB_SIGSEGV  = 5,
  KB_SIGTERM  = 6,
} kb_signal;

typedef void  (*kb_signal_handler)(int);
typedef int   (*kb_compare)(const void*, const void*);

KB_API void   kb_memset             (void* dst, uint8_t ch, size_t count);
KB_API void   kb_memcpy             (void* dst, const void* src, size_t count);
KB_API void   kb_memcpy_with_stride (void* dst, const void* src, size_t count, size_t index, size_t stride, size_t offset);
KB_API int    kb_strcmp             (const char* a, const char* b);
KB_API int    kb_strlen             (const char* a);
KB_API char*  kb_strcpy             (char* dst, const char* src);
KB_API char*  kb_strncpy            (char* dst, const char* src, size_t n);
KB_API int    kb_printf             (const char* format, ...);
KB_API int    kb_snprintf           (char* out, int32_t max, const char* format, ...);
KB_API int    kb_vsnprintf          (char* out, int32_t max, const char* format, va_list arg_list);
KB_API bool   kb_fromstr_bool       (bool*    dst, const char* str);
KB_API bool   kb_fromstr_int        (int32_t* dst, const char* str);
KB_API bool   kb_fromstr_float      (float*   dst, const char* str);
KB_API char*  kb_strdup             (const char* src);
KB_API void*  kb_memdup             (void* src, uint64_t size);
KB_API void   kb_raise              (kb_signal signal);
KB_API void   kb_set_signal_handler (kb_signal_handler handler, kb_signal signal);
KB_API void   kb_exit               (int value);
KB_API void   kb_sort               (void* data, size_t num, size_t size, kb_compare compare);

#ifdef __cplusplus
}
#endif

// #ifdef __cplusplus

// #include <fmt/format.h>

// template <typename... T>
// KB_API uint64_t kb_strfmt(char* out, int32_t size, const T&... args) {
//   auto str = fmt::format(args...);
//   int32_t len = str.length() < size ? str.length() : size;
//   kb_strncpy(out, str.c_str(), len);
//   return len;
// }

// #endif
