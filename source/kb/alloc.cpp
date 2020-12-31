// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/alloc.h>

#include <kb/log.h>
#include <kb/crt.h>
#include <kb/math.h>

#include <stdlib.h>
#include <string.h>

static const uint64_t header_check = 0x123456789ABCDEF;

struct kb_alloc_header {
  uint64_t distance;
  uint64_t check;
  uint64_t size;
};

kb_alloc_stats default_stats = {};

void* real_from_aligned(void* aligned, uint64_t distance) {
  return ((uint8_t*)aligned) - distance;
}

void* real_to_aligned(void* ptr, uint32_t header_size, uint32_t align) {
  uintptr_t address = (uintptr_t) ptr;

  uintptr_t unaligned = address + header_size;
  uintptr_t aligned = align_up(unaligned, int32_t(align));

  return (void*) aligned;
}

uint64_t total_alloc_size(uint64_t size, uint64_t align) {
  uint64_t total_align    = KB_MAX(align, sizeof(kb_alloc_header));
  uint64_t total_size     = size + total_align;
  return total_size;
}

KB_INTERNAL inline bool valid_alloc_header(void* ptr) {
  return ((kb_alloc_header*) ptr)->check == header_check;
}

KB_INTERNAL inline void write_alloc_header(void* ptr, uint64_t size, uint64_t distance) {
  ((kb_alloc_header*) ptr)->check     = size == 0 ? 0 : header_check;
  ((kb_alloc_header*) ptr)->size      = size;
  ((kb_alloc_header*) ptr)->distance  = distance;
}

KB_INTERNAL void* root_alloc(uint64_t size, uint32_t align) {
  uint64_t total_size     = total_alloc_size(size, align);
  uint8_t* real_ptr       = (uint8_t*) malloc(total_size);
  uint8_t* aligned_ptr    = (uint8_t*) real_to_aligned(real_ptr, sizeof(kb_alloc_header), align);
  uint64_t distance       = uint32_t(aligned_ptr - real_ptr);
  kb_alloc_header* header = (kb_alloc_header*) (aligned_ptr - sizeof(kb_alloc_header));

  default_stats.count++;
  default_stats.mem += size;
  write_alloc_header(header, size, distance);

  return aligned_ptr;
}

KB_INTERNAL void root_free(void* aligned_ptr) {
  kb_alloc_header* header = (kb_alloc_header*)((uint8_t*) aligned_ptr - sizeof(kb_alloc_header));
  KB_ASSERT(valid_alloc_header(header), "Unknown pointer in realloc. Invalid allocation header");
  void* real_ptr = real_from_aligned(aligned_ptr, header->distance);

  default_stats.count--;
  default_stats.mem -= header->size;
  write_alloc_header(header, 0, 0);
  
  free(real_ptr);
}

KB_INTERNAL void* root_realloc(void* ptr, uint32_t size, uint32_t align) {
  uint8_t* aligned_ptr_old    = (uint8_t*) ptr;
  uint64_t total_size         = total_alloc_size(size, align);
  kb_alloc_header* header_old = (kb_alloc_header*) (aligned_ptr_old - sizeof(kb_alloc_header));
  void* real_ptr_old          = real_from_aligned(aligned_ptr_old, header_old->distance);
      
  KB_ASSERT(valid_alloc_header(header_old), "Unknown pointer in realloc. Invalid allocation header");
  default_stats.mem += size - header_old->size;
  write_alloc_header(header_old, 0, 0);

  // ---

  uint8_t* real_ptr_new       = (uint8_t*) realloc(real_ptr_old, total_size);
  uint8_t* aligned_ptr_new    = (uint8_t*) real_to_aligned(real_ptr_new, sizeof(kb_alloc_header), align);
  uint64_t distance_new       = uint32_t(aligned_ptr_new - real_ptr_new);
  kb_alloc_header* header_new = (kb_alloc_header*) (aligned_ptr_new - sizeof(kb_alloc_header));

  write_alloc_header(header_new, size, distance_new);
  
  return aligned_ptr_new;
}

KB_INTERNAL void* default_alloc(kb_allocator* alloc, void* ptr, size_t size, size_t align) {
  if (ptr == NULL && size == 0) return NULL;

  uint8_t* res = NULL;

  if (size == 0) {
    root_free(ptr);
  } else if (ptr == NULL) {
    res = (uint8_t*) root_alloc(size, align);
  } else {
    res = (uint8_t*) root_realloc(ptr, size, align);
  }

  default_stats.high_water_mark = default_stats.mem > default_stats.high_water_mark ? default_stats.mem : default_stats.high_water_mark;

  return res;
}

KB_API void* kb_alloc(kb_allocator* alloc, size_t size, size_t align, const char* file, const char* line) {
  return (alloc ? alloc->realloc : default_alloc)(alloc, NULL, size, align);
}

KB_API void* kb_realloc(kb_allocator* alloc, void* ptr, size_t size, size_t align, const char* file, const char* line) {
  return (alloc ? alloc->realloc : default_alloc)(alloc, ptr, size, align);
}

KB_API void kb_free(kb_allocator* alloc, void* ptr, const char* file, const char* line) {
  (alloc ? alloc->realloc : default_alloc)(alloc, ptr, 0, 0);
}

KB_API uint32_t kb_alloc_count(kb_allocator* alloc) {
  return (alloc ? alloc->stats : default_stats).count;
}

KB_API uint64_t kb_alloc_mem(kb_allocator* alloc) {
  return (alloc ? alloc->stats : default_stats).mem;
}

KB_API uint64_t kb_alloc_high_water_mark(kb_allocator* alloc) {
  return (alloc ? alloc->stats : default_stats).high_water_mark;
}
