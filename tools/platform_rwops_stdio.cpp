// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>
#include <kb/log.h>

#include <stdio.h>

KB_INTERNAL const char* cv_rwops_mode(kb_file_mode mode) {
  switch (mode) {
    case KB_FILE_MODE_READ:   return "rb";
    case KB_FILE_MODE_WRITE:  return "wb";
    default:                  return "";
  }
}

KB_INTERNAL int64_t size_impl_file(kb_rwops* rwops) {
  int64_t pos, size;
  
  pos = kb_rwops_tell(rwops);
  if (pos < 0) return -1;
  size = kb_rwops_seek(rwops, 0, KB_RWOPS_SEEK_END);
  kb_rwops_seek(rwops, pos, KB_RWOPS_SEEK_BEG);

  return size;
}

KB_INTERNAL int64_t seek_impl_file(kb_rwops* rwops, int64_t offset, kb_whence whence) {
  if (fseek((FILE*) rwops->impl, offset, whence) == 0) {
    int64_t pos = ftell((FILE*) rwops->impl);
    return pos;
  }
  return -1;
}

KB_INTERNAL int64_t read_impl_file(kb_rwops* rwops, void *ptr, int64_t size, int64_t count) {
  int64_t read = fread(ptr, size, count, (FILE*) rwops->impl);
  return read * size;
}

KB_INTERNAL int64_t write_impl_file(kb_rwops* rwops, const void* ptr, int64_t size, int64_t count) {
  int64_t wrote = fwrite(ptr, size, count, (FILE*) rwops->impl);
  return wrote * size;
}

KB_INTERNAL int64_t tell_impl_file(kb_rwops* rwops) {
  return ftell((FILE*) rwops->impl);
}

KB_INTERNAL int close_impl_file(kb_rwops* rwops) {
  int res = fclose((FILE*) rwops->impl);
  KB_DEFAULT_FREE(rwops);
  return res;
}

KB_INTERNAL int64_t size_impl_mem(kb_rwops* rwops) {
  return rwops->mem_size;
}

KB_INTERNAL int64_t seek_impl_mem(kb_rwops* rwops, int64_t offset, kb_whence whence) {
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

KB_INTERNAL int64_t read_impl_mem(kb_rwops* rwops, void *dst, int64_t size, int64_t count) {
  if ((count <= 0) || (size <= 0)) return 0;
  
  size_t avail = rwops->mem_size - rwops->mem_pos;
  size_t wanted = (count * size);
  size_t byte_count = wanted > avail ? avail : wanted;
  
  kb_memcpy(dst, rwops->mem_ptr + rwops->mem_pos, byte_count);
  
  rwops->mem_pos += byte_count;
  
  return byte_count;
}

KB_INTERNAL int64_t write_impl_mem(kb_rwops* rwops, const void* src, int64_t size, int64_t count) {
  if ((rwops->mem_pos + (count * size)) > rwops->mem_size) {
    count = (rwops->mem_size - rwops->mem_pos) / size;
  }

  kb_memcpy(rwops->mem_ptr + rwops->mem_pos, src, count * size);
  
  rwops->mem_pos += size * count;
  
  return size * count;
}

KB_INTERNAL int64_t tell_impl_mem(kb_rwops* rwops) {
  return rwops->mem_pos;
}

KB_INTERNAL int close_impl_mem(kb_rwops* rwops) {
  KB_DEFAULT_FREE(rwops);
  return 0;
}

KB_INTERNAL kb_rwops* create_rwops_mem(char* ptr, uint64_t size) {
  kb_rwops* rwops = KB_DEFAULT_ALLOC_TYPE(kb_rwops, 1);
  rwops->size     = size_impl_mem;
  rwops->seek     = seek_impl_mem;
  rwops->read     = read_impl_mem;
  rwops->close    = close_impl_mem;
  rwops->write    = write_impl_mem;
  rwops->tell     = tell_impl_mem;
  rwops->impl     = NULL;
  rwops->mem_ptr  = ptr;
  rwops->mem_pos  = 0;
  rwops->mem_size = size;
  
  return rwops;
}
KB_INTERNAL kb_rwops* create_rwops_file(FILE* impl) {
  kb_rwops* rwops = KB_DEFAULT_ALLOC_TYPE(kb_rwops, 1);
  rwops->size     = size_impl_file;
  rwops->seek     = seek_impl_file;
  rwops->read     = read_impl_file;
  rwops->close    = close_impl_file;
  rwops->write    = write_impl_file;
  rwops->tell     = tell_impl_file;
  rwops->impl     = impl;
  rwops->mem_pos  = 0;
  rwops->mem_size = 0;
  
  return rwops;
}

KB_API kb_rwops* kb_rwops_open_file(const char* path, kb_file_mode mode) {
  FILE *impl = fopen(path, "r");

  if (!impl) return NULL;
      
  return create_rwops_file(impl);
}


KB_API kb_rwops* kb_rwops_open_mem(void* dst, int64_t size) {
  if (!dst) return nullptr;
  return create_rwops_mem((char*) dst, size);
}
