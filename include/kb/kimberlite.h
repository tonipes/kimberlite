#pragma once

#include <kb/alloc.h>

// Alloc

KB_API Allocator* kb_temporary_alloc();
KB_API Allocator* kb_persistent_alloc();

#define KB_TEMP_ALLOC(size)                                     kb_alloc    (kb_temporary_alloc(), size,                       KB_DEFAULT_ALIGN)
#define KB_TEMP_ALLOC_TYPE(type, count)                 (type*) kb_alloc    (kb_temporary_alloc(), sizeof(type) * count,       KB_DEFAULT_ALIGN)

#define KB_PERSISTENT_ALLOC(size)                               kb_alloc    (kb_persistent_alloc(), size,                       KB_DEFAULT_ALIGN)
#define KB_PERSISTENT_ALLOC_TYPE(type, count)           (type*) kb_alloc    (kb_persistent_alloc(), sizeof(type) * count,       KB_DEFAULT_ALIGN)
#define KB_PERSISTENT_REALLOC_TYPE(type, ptr, count)    (type*) kb_realloc  (kb_persistent_alloc(), ptr, sizeof(type) * count,  KB_DEFAULT_ALIGN)
#define KB_PERSISTENT_REALLOC(ptr, size)                        kb_realloc  (kb_persistent_alloc(), ptr, size,                  KB_DEFAULT_ALIGN)
#define KB_PERSISTENT_FREE(ptr)                                 kb_free     (kb_persistent_alloc(), ptr,                        KB_DEFAULT_ALIGN)



KB_API void kb_init();
KB_API void kb_deinit();
KB_API void kb_frame();
