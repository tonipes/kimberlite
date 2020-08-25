
inline uint32_t hash_key(uint32_t k) {
  const uint32_t tmp0 = k * 2246822519U;
  const uint32_t tmp1 = (tmp0 << 13) | (tmp0 >> (32-13));
  return tmp1 * 2654435761U;
}

template <uint16_t MaxHandles>
HandleAllocator<MaxHandles>::HandleAllocator(): handle_count(0), capacity(MaxHandles) {
  reset();
}

template <uint16_t MaxHandles>
HandleAllocator<MaxHandles>::~HandleAllocator() {

}

template <uint16_t MaxHandles>
auto HandleAllocator<MaxHandles>::alloc() -> Handle {
  if (handle_count < capacity) {
    uint16_t index = handle_count;
    handle_count++;

    uint16_t* dense  = dense_ptr();
    uint16_t* sparse = sparse_ptr();

    uint16_t handle = dense[index];
    sparse[handle] = index;
    
    return handle;
  }

  return InvalidHandle;
}

template <uint16_t MaxHandles>
auto HandleAllocator<MaxHandles>::dense_ptr() -> Handle* {
  return &handles[0];
}

template <uint16_t MaxHandles>
auto HandleAllocator<MaxHandles>::sparse_ptr() -> Handle* {
  return &dense_ptr()[MaxHandles];
}

template <uint16_t MaxHandles>
auto HandleAllocator<MaxHandles>::free(Handle handle) -> void {
  uint16_t* dense  = dense_ptr();
  uint16_t* sparse = sparse_ptr();
  uint16_t index = sparse[handle];
  handle_count--;
  uint16_t temp = dense[handle_count];
  dense[handle_count] = handle;
  sparse[temp] = index;
  dense[index] = temp;
}

template <uint16_t MaxHandles>
auto HandleAllocator<MaxHandles>::reset() -> void {
  handle_count = 0;
  uint16_t* dense = dense_ptr();
  for (uint16_t ii = 0, num = MaxHandles; ii < num; ++ii) {
    dense[ii] = ii;
  }
}

template <uint16_t MaxHandles>
auto HandleAllocator<MaxHandles>::count() const -> uint32_t {
  return handle_count;
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::insert(K key, Handle handle) -> bool { 
  if (handle == InvalidHandle) { return false; }
  
  const K hash = hash_key(key);
  const uint32_t first_idx = hash % MaxHandles;
  uint32_t idx = first_idx;
  
  do {
    if (handles[idx] == InvalidHandle) {
      keys[idx]    = key;
      handles[idx] = handle;
      elem_count++;
      return true;
    }
		
    if (keys[idx] == key) { return false; }
    
    idx = (idx + 1) % InvalidHandle;

  } while (idx != first_idx);

  return false;
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::find_index(K key) const -> uint32_t {
  const K hash = hash_key(key);
  const uint32_t first_idx = hash % MaxHandles;
  uint32_t idx = first_idx;
	
  do {
    if (handles[idx] == InvalidHandle) {
      return UINT32_MAX;
    }

    if (keys[idx] == key) {
      return idx;
    }

    idx = (idx + 1) % MaxHandles;

  } while (idx != first_idx);
  
  return UINT32_MAX;
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::find(K key) const -> Handle {
  const uint32_t idx = find_index(key);
  if (UINT32_MAX != idx) {
    return handles[idx];
  }

  return InvalidHandle;
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::has(K key) const -> bool {
  return find(key) != InvalidHandle;
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::remove(Handle handle) -> void {
  if (handle != InvalidHandle) {
    for (uint32_t i = 0; i < MaxHandles; i++) {
      if (handles[i] == handle) {
        remove_index(i);
      }
    }
  }
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::reset() -> void {
  for (uint32_t i = 0; i < MaxHandles; i++) {
    handles[i] = InvalidHandle;
  }

  elem_count = 0;
}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::count() const -> uint32_t {
  return elem_count;
}

template <uint16_t MaxHandles, typename K>
HandleTable<MaxHandles, K>::HandleTable(): capacity(MaxHandles) {
  reset();
}

template <uint16_t MaxHandles, typename K>
HandleTable<MaxHandles, K>::~HandleTable() {

}

template <uint16_t MaxHandles, typename K>
auto HandleTable<MaxHandles, K>::remove_index(uint32_t idx) -> void {
  handles[idx] = InvalidHandle;
  elem_count--;

  const uint32_t s = (idx + 1) % MaxHandles;

  for (uint32_t i = s; handles[i] != InvalidHandle; i = (i + 1) % MaxHandles) {
		if (handles[i] != InvalidHandle) {
      const K key = keys[i];
      if (find(key) != i) {
        const uint16_t handle = handles[i];
        handles[i] = InvalidHandle;
        elem_count--;
        insert(key, handle);
      }
    }
  }

}

