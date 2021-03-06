// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/foundation/crt.h>
#include <kb/foundation/alloc.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

KB_API void kb_memset(void* dst, uint8_t ch, size_t count) {
  memset(dst, ch, count);
}

KB_API void kb_memcpy(void* dst, const void* src, size_t count) {
  memcpy(dst, src, count);
}

KB_API void kb_memcpy_with_stride(void* dst, const void* src, size_t count, size_t index, size_t stride, size_t offset) {
  uint8_t* data = (uint8_t*) dst + index * stride + offset; 
  kb_memcpy(data, src, count);
}

KB_API int kb_strcmp(const char* a, const char* b) {
  return strcmp(a, b);
}

KB_API uint64_t kb_strlen(const char* a) {
  return strlen(a);
}

KB_API char* kb_strcpy(char* dst, const char* src) {
  return strcpy(dst, src);
}

KB_API char* kb_strncpy(char* dst, const char* src, size_t n) {
  return strncpy(dst, src, n);
}

KB_API int kb_printf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  int total = vprintf(format, args);
  va_end(args);

  return total;
}

KB_API int kb_vsnprintf(char* out, int32_t max, const char* format, va_list arg_list) {
  va_list args;
  va_copy(args, arg_list);
  int32_t total = vsnprintf(out, max, format, args);
  va_end(args);

  return total;
}

KB_API int kb_snprintf(char* out, int32_t max, const char* format, ...) {
  va_list args;
  va_start(args, format);
  int32_t total = kb_vsnprintf(out, max, format, args);
  va_end(args);

  return total;
}

KB_API bool kb_fromstr_bool(bool* dst, const char* str) {
  char ch = str[0];
  *dst = ch == 't' ||  ch == '1';
  return kb_strlen(str) != 0; 
}

// TODO: Not safe. Converts from long to int
KB_API bool kb_fromstr_int(int32_t* dst, const char* str) {
  char* end = nullptr;
  long tmp = strtol(str, &end, 0);
  *dst = (int32_t) tmp;
  return true;
}

// TODO: 
KB_API bool kb_fromstr_float(float* dst, const char* str) {
  *dst = atof(str);
  return true;
}

char* KB_API kb_strdup(const char* src) {
  char* c = (char*) KB_DEFAULT_ALLOC(kb_strlen(src) + 1);
  strcpy(c, src);
  return c;
}

KB_API void* kb_memdup(const void* src, uint64_t size) {
  void* p = KB_DEFAULT_ALLOC(size);
  kb_memcpy(p, src, size);
  return p;
}

KB_API void kb_sort(void* data, size_t num, size_t size, kb_compare compare) {
  qsort(data, num, size, compare);
}
