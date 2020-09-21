#pragma once

#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t Handle;

static const Handle InvalidHandle = UINT16_MAX;

static inline bool is_valid_handle(const Handle handle) { return handle != InvalidHandle; }

#define KB_HANDLE(name)                                                                           \
	typedef struct {                                                                                \
    uint16_t idx;                                                                                 \
  } name;                                                                                         \
  static inline bool is_valid##name(const name handle) { return handle.idx != InvalidHandle; }    \
  static const name Invalid##name = { InvalidHandle };                                            \

#define KB_INVALID_HANDLE { InvalidHandle }

#define INVALID_HANDLE_CHECK(name, handle) if (!is_valid_handle(handle.idx)) kb_log_error("Using invalid {} handle", #name);

#ifdef __cplusplus
}
#endif

// c++ implementation
#ifdef __cplusplus

#include <kb/hash.h>

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

#endif
