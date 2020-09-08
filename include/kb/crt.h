#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void kb_memset(void* dst, uint8_t ch, size_t count);
void kb_memcpy(void* dst, const void* src, size_t count);
void kb_memcpy_with_stride(void* dst, const void* src, size_t count, size_t index, size_t stride, size_t offset);

int   kb_strcmp(const char* a, const char* b);
int   kb_strlen(const char* a);
char* kb_strcpy(char* dst, const char* src);

int kb_printf(const char* format, ...);
int kb_snprintf(char* out, int32_t max, const char* format, ...);

bool kb_fromstr_bool  (bool*    dst, const char* str);
bool kb_fromstr_int   (int32_t* dst, const char* str);
bool kb_fromstr_float (float*   dst, const char* str);

#ifdef __cplusplus
}
#endif
