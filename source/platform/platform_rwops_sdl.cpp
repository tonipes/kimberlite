// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <SDL2/SDL.h>

KB_INTERNAL const char* cv_rwops_mode(kb_file_mode mode) {
  switch (mode) {
    case KB_FILE_MODE_READ:   return "rb";
    case KB_FILE_MODE_WRITE:  return "wb";
    default:                  return "";
  }
}

KB_INTERNAL int cv_rwops_whence(kb_whence whence) {
  switch (whence) {
    case KB_RWOPS_SEEK_BEG:  return RW_SEEK_SET;
    case KB_RWOPS_SEEK_CUR:  return RW_SEEK_CUR;
    case KB_RWOPS_SEEK_END:  return RW_SEEK_END;
    default:                 return RW_SEEK_SET;
  }
}

KB_INTERNAL int64_t size_impl(kb_rwops* rwops) {
  return SDL_RWsize((SDL_RWops*) rwops->impl);
}

KB_INTERNAL int64_t seek_impl(kb_rwops* rwops, int64_t offset, int whence) {
  return SDL_RWseek((SDL_RWops*) rwops->impl, offset, whence);
}

KB_INTERNAL uint64_t read_impl(kb_rwops* rwops, void *ptr, uint64_t size) {
  return SDL_RWread((SDL_RWops*) rwops->impl, ptr, 1, size);
}

KB_INTERNAL uint64_t write_impl(kb_rwops* rwops, const void* ptr, uint64_t size) {
  return SDL_RWwrite((SDL_RWops*) rwops->impl, ptr, 1, size);
}

KB_INTERNAL int64_t tell_impl(kb_rwops* rwops) {
  return SDL_RWtell((SDL_RWops*) rwops->impl);
}

KB_INTERNAL int close_impl(kb_rwops* rwops) {
  int res = SDL_RWclose((SDL_RWops*) rwops->impl);
  KB_DEFAULT_FREE(rwops);
  return res;
}

KB_INTERNAL inline kb_rwops* create_rwops(SDL_RWops* impl) {
  kb_rwops* rwops = KB_DEFAULT_ALLOC_TYPE(kb_rwops, 1);
  rwops->size   = size_impl;
  rwops->seek   = seek_impl;
  rwops->read   = read_impl;
  rwops->close  = close_impl;
  rwops->write  = write_impl;
  rwops->tell   = tell_impl;
  rwops->impl   = impl;
  
  return rwops;
}

KB_API kb_rwops* kb_rwops_open_file(const char* path, kb_file_mode mode) {
  SDL_RWops* impl = SDL_RWFromFile(path, cv_rwops_mode(mode));
  if (!impl) return nullptr;
  return create_rwops(impl);
}

KB_API kb_rwops* kb_rwops_open_mem(const void* dst, uint64_t size) {
  SDL_RWops* impl = SDL_RWFromConstMem(dst, size); // TODO: Mode
  if (!impl) return nullptr;
  return create_rwops(impl);
}
