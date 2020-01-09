// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Document/JsonDocument.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Helper to implement the "base-from-member" idiom
// (we need to store the allocator before constructing JsonDocument)
template <typename TAllocator>
class AllocatorOwner {
 protected:
  AllocatorOwner() {}
  AllocatorOwner(const AllocatorOwner& src) : _allocator(src._allocator) {}
  AllocatorOwner(TAllocator allocator) : _allocator(allocator) {}

  void* allocate(size_t size) {
    return _allocator.allocate(size);
  }

  void deallocate(void* ptr) {
    _allocator.deallocate(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return _allocator.reallocate(ptr, new_size);
  }

 private:
  TAllocator _allocator;
};

template <typename TAllocator>
class BasicJsonDocument : AllocatorOwner<TAllocator>, public JsonDocument {
 public:
  explicit BasicJsonDocument(size_t capa, TAllocator allocator = TAllocator())
      : AllocatorOwner<TAllocator>(allocator), JsonDocument(allocPool(capa)) {}

  BasicJsonDocument(const BasicJsonDocument& src)
      : AllocatorOwner<TAllocator>(src),
        JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  template <typename T>
  BasicJsonDocument(const T& src,
                    typename enable_if<IsVisitable<T>::value>::type* = 0)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  // disambiguate
  BasicJsonDocument(VariantRef src)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  ~BasicJsonDocument() {
    freePool();
  }

  BasicJsonDocument& operator=(const BasicJsonDocument& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    set(src);
    return *this;
  }

  template <typename T>
  BasicJsonDocument& operator=(const T& src) {
    reallocPoolIfTooSmall(src.memoryUsage());
    set(src);
    return *this;
  }

  void shrinkToFit() {
    ptrdiff_t bytes_reclaimed = _pool.squash();
    if (bytes_reclaimed == 0) return;

    void* old_ptr = _pool.buffer();
    void* new_ptr = this->reallocate(old_ptr, _pool.capacity());

    ptrdiff_t ptr_offset =
        static_cast<char*>(new_ptr) - static_cast<char*>(old_ptr);

    _pool.movePointers(ptr_offset);
    _data.movePointers(ptr_offset, ptr_offset - bytes_reclaimed);
  }

 private:
  MemoryPool allocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
    return MemoryPool(reinterpret_cast<char*>(this->allocate(capa)), capa);
  }

  void reallocPoolIfTooSmall(size_t requiredSize) {
    if (requiredSize <= capacity()) return;
    freePool();
    replacePool(allocPool(addPadding(requiredSize)));
  }

  void freePool() {
    this->deallocate(memoryPool().buffer());
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
