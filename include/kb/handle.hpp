#pragma once

#include "handle.h"
#include "hash.h"

template <uint16_t MaxHandles>
class HandleAllocator {
public:
  HandleAllocator();
  ~HandleAllocator();

  auto alloc  ()                      -> Handle;
  auto has    (Handle handle)   const -> bool;
  auto free   (Handle handle)         -> void;
  auto reset  ()                      -> void;
  auto count  ()                const -> uint32_t;

private:
  auto dense_ptr()                    -> Handle*;
  auto sparse_ptr()                   -> Handle*;

  Handle    handles[2 * MaxHandles];
  uint16_t  handle_count;
  uint16_t  capacity;
};

template <uint16_t MaxHandles, typename K = uint32_t>
class HandleTable {
public:
  HandleTable();
  ~HandleTable();

  auto insert (K key, Handle handle)        -> bool;
  auto find   (K key)                 const -> Handle;
  auto remove (Handle handle)               -> void;
  auto has    (K key)                 const -> bool;
  auto reset  ()                            -> void;
  auto count  ()                      const -> uint32_t;

private:
  auto remove_index(uint32_t idx)           -> void;
  auto find_index(K key)              const -> uint32_t;

  uint32_t  capacity;
  uint32_t  elem_count;
  K         keys[MaxHandles];
  Handle    handles[MaxHandles];
};

#include "inline/handle.inl"
