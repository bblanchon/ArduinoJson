// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "JsonDocument.hpp"

namespace ARDUINOJSON_NAMESPACE {

template <typename TAllocator>
class AllocatorOwner {
 protected:
  AllocatorOwner() {}
  AllocatorOwner(const AllocatorOwner& src) : _allocator(src._allocator) {}
  AllocatorOwner(TAllocator allocator) : _allocator(allocator) {}

  void* allocate(size_t n) {
    return _allocator.allocate(n);
  }

  void deallocate(void* p) {
    _allocator.deallocate(p);
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
