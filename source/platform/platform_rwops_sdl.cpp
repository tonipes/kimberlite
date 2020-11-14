// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/rwops.h>
#include <kb/alloc.h>

#include <SDL2/SDL.h>

const char* cv_file_mode[] {
  "",
  "rb",
  "wb",
};

int64_t size_impl(kb_rwops* rwops) {
  return SDL_RWsize((SDL_RWops*) rwops->impl);
}

int64_t seek_impl(kb_rwops* rwops, int64_t offset, int whence) {
  return SDL_RWseek((SDL_RWops*) rwops->impl, offset, whence);
}

uint64_t read_impl(kb_rwops* rwops, void *ptr, uint64_t size) {
  return SDL_RWread((SDL_RWops*) rwops->impl, ptr, 1, size);
}

uint64_t write_impl(kb_rwops* rwops, const void* ptr, uint64_t size) {
  return SDL_RWwrite((SDL_RWops*) rwops->impl, ptr, 1, size);
}

int64_t tell_impl(kb_rwops* rwops) {
  return SDL_RWtell((SDL_RWops*) rwops->impl);
}

int close_impl(kb_rwops* rwops) {
  int res = SDL_RWclose((SDL_RWops*) rwops->impl);
  KB_DEFAULT_FREE(rwops);
  return res;
}

inline kb_rwops* create_rwops(SDL_RWops* impl) {
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
  SDL_RWops* impl = SDL_RWFromFile(path, cv_file_mode[mode]); // TODO: Mode
  if (!impl) return nullptr;
  return create_rwops(impl);
}

KB_API kb_rwops* kb_rwops_open_mem(const void* dst, uint64_t size) {
  SDL_RWops* impl = SDL_RWFromConstMem(dst, size); // TODO: Mode
  if (!impl) return nullptr;
  return create_rwops(impl);
}
