// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/stream.h>
#include <kb/alloc.h>
#include <kb/crt.h>
#include <kb/platform.h>

KB_INTERNAL int64_t size_impl_mem(kb_stream* rwops) {
  return rwops->mem_size;
}

KB_INTERNAL int64_t seek_impl_mem(kb_stream* rwops, int64_t offset, kb_whence whence) {
  uint64_t pos = 0;
  
  switch (whence) {
    case KB_RWOPS_SEEK_BEG: {
      pos = offset;
    } break;
    case KB_RWOPS_SEEK_CUR: {
      pos = rwops->mem_pos + offset;
    } break;
    case KB_RWOPS_SEEK_END:{
      pos = rwops->mem_size + offset;
    } break;
  }
  
  if (pos < 0) pos = 0;
  if (pos > rwops->mem_size) pos = rwops->mem_size;
  
  rwops->mem_pos = pos;
  return pos;
}

KB_INTERNAL int64_t read_impl_mem(kb_stream* rwops, void *dst, int64_t size, int64_t count) {
  if ((count <= 0) || (size <= 0)) return 0;
  
  size_t avail = rwops->mem_size - rwops->mem_pos;
  size_t wanted = (count * size);
  size_t byte_count = wanted > avail ? avail : wanted;
  
  kb_memcpy(dst, rwops->mem_ptr + rwops->mem_pos, byte_count);
  
  rwops->mem_pos += byte_count;
  
  return byte_count;
}

KB_INTERNAL int64_t write_impl_mem(kb_stream* rwops, const void* src, int64_t size, int64_t count) {
  if ((rwops->mem_pos + (count * size)) > rwops->mem_size) {
    count = (rwops->mem_size - rwops->mem_pos) / size;
  }

  kb_memcpy(rwops->mem_ptr + rwops->mem_pos, src, count * size);
  
  rwops->mem_pos += size * count;
  
  return size * count;
}

KB_INTERNAL int64_t tell_impl_mem(kb_stream* rwops) {
  return rwops->mem_pos;
}

KB_INTERNAL int close_impl_mem(kb_stream* rwops) {
  return 0;
}

KB_API kb_stream* kb_stream_open_mem(void* ptr, int64_t size) {
  if (!ptr) return nullptr;

  kb_stream* stream = KB_DEFAULT_ALLOC_TYPE(kb_stream, 1);

  stream->size     = size_impl_mem;
  stream->seek     = seek_impl_mem;
  stream->read     = read_impl_mem;
  stream->close    = close_impl_mem;
  stream->write    = write_impl_mem;
  stream->tell     = tell_impl_mem;
  stream->impl     = NULL;
  stream->mem_ptr  = (char*) ptr;
  stream->mem_pos  = 0;
  stream->mem_size = size;

  return stream;
}

KB_API kb_stream* kb_stream_open_file(const char* path, kb_file_mode mode) {
  kb_stream* stream = KB_DEFAULT_ALLOC_TYPE(kb_stream, 1);
  int res = kb_platform_stream_file_open(stream, path, mode);
  
  if (res != 0) {
    KB_DEFAULT_FREE(stream);
    return 0;
  }

  return stream;
}
