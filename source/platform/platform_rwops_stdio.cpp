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

KB_INTERNAL int64_t seek_impl_file(kb_rwops* rwops, int64_t offset, int whence) {
  if (fseek((FILE*) rwops->impl, offset, whence) == 0) {
    int64_t pos = ftell((FILE*) rwops->impl);
    return pos;
  }
  return -1;
}

KB_INTERNAL uint64_t read_impl_file(kb_rwops* rwops, void *ptr, uint64_t size) {
  uint64_t read = fread(ptr, size, 1, (FILE*) rwops->impl);
  return read;
}

KB_INTERNAL uint64_t write_impl_file(kb_rwops* rwops, const void* ptr, uint64_t size) {
  uint64_t wrote = fwrite(ptr, size, 1, (FILE*) rwops->impl);
  return wrote;
}

KB_INTERNAL int64_t tell_impl_file(kb_rwops* rwops) {
  return ftell((FILE*) rwops->impl);
}

KB_INTERNAL int close_impl_file(kb_rwops* rwops) {
  int res = fclose((FILE*) rwops->impl);
  KB_DEFAULT_FREE(rwops);
  return res;
}

KB_INTERNAL inline kb_rwops* create_rwops_file(FILE* impl) {
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
  FILE* impl = fopen(path, cv_rwops_mode(mode));
  if (!impl) return nullptr;      
  return create_rwops_file(impl);
}


// KB_INTERNAL int64_t size_impl_mem(kb_rwops* rwops) {
//   int64_t pos, size;
  
//   // pos = kb_rwops_seek(rwops, 0, KB_RWOPS_SEEK_CUR);
//   // if (pos < 0) return -1;
//   // size = kb_rwops_seek(rwops, 0, KB_RWOPS_SEEK_END);
//   // kb_rwops_seek(rwops, pos, KB_RWOPS_SEEK_CUR);

//   return size;
// }

// KB_INTERNAL int64_t seek_impl_mem(kb_rwops* rwops, int64_t offset, int whence) {
//   // if (fseek((FILE*) rwops->impl, offset, whence) == 0) {
//   //   int64_t pos = ftell((FILE*) rwops->impl);
//   //   return pos;
//   // }
//   return -1;
// }

// KB_INTERNAL uint64_t read_impl_mem(kb_rwops* rwops, void *ptr, uint64_t size) {
//   uint64_t read = fread(ptr, size, 1, (FILE*) rwops->impl);
//   return read;
// }

// KB_INTERNAL uint64_t write_impl_mem(kb_rwops* rwops, const void* ptr, uint64_t count) {
//   if (!rwops) return 0;

//   uint64_t write_count = MIN(rwops->mem_size - rwops->mem_pos, count);
  
//   kb_memcpy(rwops->impl, ptr, write_count);

//   return write_count;
// }

// KB_INTERNAL int64_t tell_impl_mem(kb_rwops* rwops) {
//   // return ftell((FILE*) rwops->impl);
// }

// KB_INTERNAL int close_impl_mem(kb_rwops* rwops) {
//   if (!rwops) return 0;
  
//   KB_DEFAULT_FREE(rwops);
//   return 0;
// }

// KB_INTERNAL inline kb_rwops* create_rwops_mem(void* ptr, uint64_t size) {
//   kb_rwops* rwops = KB_DEFAULT_ALLOC_TYPE(kb_rwops, 1);
//   rwops->size   = size_impl_mem;
//   rwops->seek   = seek_impl_mem;
//   rwops->read   = read_impl_mem;
//   rwops->close  = close_impl_mem;
//   rwops->write  = write_impl_mem;
//   rwops->tell   = tell_impl_mem;
//   rwops->impl   = ptr;
//   rwops->pos    = 0;
//   rwops->size   = size;

//   return rwops;
// }

// KB_API kb_rwops* kb_rwops_open_mem(void* dst, uint64_t size) {
//   if (!dst) return nullptr;
//   return create_rwops_mem(dst, size);
// }
