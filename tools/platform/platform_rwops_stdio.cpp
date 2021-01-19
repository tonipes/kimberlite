// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <stdio.h>

KB_INTERNAL const char* cv_rwops_mode(kb_file_mode mode) {
  switch (mode) {
    case KB_FILE_MODE_READ:   return "rb";
    case KB_FILE_MODE_WRITE:  return "wb";
    default:                  return "";
  }
}

KB_INTERNAL int64_t size_impl_file(kb_stream* rwops) {
  int64_t pos, size;
  
  pos = kb_stream_tell(rwops);
  if (pos < 0) return -1;
  size = kb_stream_seek(rwops, 0, KB_RWOPS_SEEK_END);
  kb_stream_seek(rwops, pos, KB_RWOPS_SEEK_BEG);

  return size;
}

KB_INTERNAL int64_t seek_impl_file(kb_stream* rwops, int64_t offset, kb_whence whence) {
  if (fseek((FILE*) rwops->impl, offset, whence) == 0) {
    int64_t pos = ftell((FILE*) rwops->impl);
    return pos;
  }
  return -1;
}

KB_INTERNAL int64_t read_impl_file(kb_stream* rwops, void *ptr, int64_t size, int64_t count) {
  int64_t read = fread(ptr, size, count, (FILE*) rwops->impl);
  return read * size;
}

KB_INTERNAL int64_t write_impl_file(kb_stream* rwops, const void* ptr, int64_t size, int64_t count) {
  int64_t wrote = fwrite(ptr, size, count, (FILE*) rwops->impl);
  return wrote * size;
}

KB_INTERNAL int64_t tell_impl_file(kb_stream* rwops) {
  return ftell((FILE*) rwops->impl);
}

KB_INTERNAL int close_impl_file(kb_stream* rwops) {
  int res = fclose((FILE*) rwops->impl);
  return res;
}

KB_INTERNAL kb_stream* create_rwops_file(FILE* impl) {
  kb_stream* rwops = KB_DEFAULT_ALLOC_TYPE(kb_stream, 1);
  rwops->size     = size_impl_file;
  rwops->seek     = seek_impl_file;
  rwops->read     = read_impl_file;
  rwops->close    = close_impl_file;
  rwops->write    = write_impl_file;
  rwops->tell     = tell_impl_file;
  rwops->impl     = impl;
  rwops->mem_ptr  = NULL;
  rwops->mem_pos  = 0;
  rwops->mem_size = 0;
  
  return rwops;
}

KB_API kb_stream* kb_stream_open_file(const char* path, kb_file_mode mode) {
  FILE *impl = fopen(path, cv_rwops_mode(mode));

  if (!impl) return NULL;
      
  return create_rwops_file(impl);
}
