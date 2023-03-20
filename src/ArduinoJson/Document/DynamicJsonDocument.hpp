// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/JsonDocument.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>

#include <stdlib.h>  // malloc, free

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Helper to implement the "base-from-member" idiom
// (we need to store the allocator before constructing JsonDocument)
class AllocatorOwner {
 public:
  AllocatorOwner(Allocator* allocator) : _allocator(allocator) {}

  void* allocate(size_t size) {
    return _allocator->allocate(size);
  }

  void deallocate(void* ptr) {
    if (ptr)
      _allocator->deallocate(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return _allocator->reallocate(ptr, new_size);
  }

  Allocator* allocator() {
    return _allocator;
  }

 private:
  Allocator* _allocator;
};

// A JsonDocument that uses the provided allocator to allocate its memory pool.
// https://arduinojson.org/v6/api/dynamicjsondocument/
class DynamicJsonDocument : AllocatorOwner, public JsonDocument {
 public:
  explicit DynamicJsonDocument(
      size_t capa, Allocator* alloc = detail::DefaultAllocator::instance())
      : AllocatorOwner(alloc), JsonDocument(allocPool(capa)) {}

  // Copy-constructor
  DynamicJsonDocument(const DynamicJsonDocument& src)
      : AllocatorOwner(src), JsonDocument() {
    copyAssignFrom(src);
  }

  // Move-constructor
  DynamicJsonDocument(DynamicJsonDocument&& src) : AllocatorOwner(src) {
    moveAssignFrom(src);
  }

  DynamicJsonDocument(const JsonDocument& src)
      : AllocatorOwner(detail::DefaultAllocator::instance()) {
    copyAssignFrom(src);
  }

  // Construct from variant, array, or object
  template <typename T>
  DynamicJsonDocument(
      const T& src, typename detail::enable_if<
                        detail::is_same<T, JsonVariant>::value ||
                        detail::is_same<T, JsonVariantConst>::value ||
                        detail::is_same<T, JsonArray>::value ||
                        detail::is_same<T, JsonArrayConst>::value ||
                        detail::is_same<T, JsonObject>::value ||
                        detail::is_same<T, JsonObjectConst>::value>::type* = 0)
      : DynamicJsonDocument(src.memoryUsage()) {
    set(src);
  }

  // disambiguate
  DynamicJsonDocument(JsonVariant src)
      : AllocatorOwner(detail::DefaultAllocator::instance()),
        JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  ~DynamicJsonDocument() {
    freePool();
  }

  DynamicJsonDocument& operator=(const DynamicJsonDocument& src) {
    copyAssignFrom(src);
    return *this;
  }

  DynamicJsonDocument& operator=(DynamicJsonDocument&& src) {
    moveAssignFrom(src);
    return *this;
  }

  template <typename T>
  DynamicJsonDocument& operator=(const T& src) {
    size_t requiredSize = src.memoryUsage();
    if (requiredSize > capacity())
      reallocPool(requiredSize);
    set(src);
    return *this;
  }

  // Reduces the capacity of the memory pool to match the current usage.
  // https://arduinojson.org/v6/api/dynamicjsondocument/shrinktofit/
  void shrinkToFit() {
    ptrdiff_t bytes_reclaimed = _pool.squash();
    if (bytes_reclaimed == 0)
      return;

    void* old_ptr = _pool.buffer();
    void* new_ptr = this->reallocate(old_ptr, _pool.capacity());

    ptrdiff_t ptr_offset =
        static_cast<char*>(new_ptr) - static_cast<char*>(old_ptr);

    _pool.movePointers(ptr_offset);
    _data.movePointers(ptr_offset, ptr_offset - bytes_reclaimed);
  }

  // Reclaims the memory leaked when removing and replacing values.
  // https://arduinojson.org/v6/api/jsondocument/garbagecollect/
  bool garbageCollect() {
    // make a temporary clone and move assign
    DynamicJsonDocument tmp(*this);
    if (!tmp.capacity())
      return false;
    tmp.set(*this);
    moveAssignFrom(tmp);
    return true;
  }

  using AllocatorOwner::allocator;

 private:
  detail::MemoryPool allocPool(size_t requiredSize) {
    size_t capa = detail::addPadding(requiredSize);
    return {reinterpret_cast<char*>(this->allocate(capa)), capa};
  }

  void reallocPool(size_t requiredSize) {
    size_t capa = detail::addPadding(requiredSize);
    if (capa == _pool.capacity())
      return;
    freePool();
    replacePool(allocPool(detail::addPadding(requiredSize)));
  }

  void freePool() {
    this->deallocate(getPool()->buffer());
  }

  void copyAssignFrom(const JsonDocument& src) {
    reallocPool(src.capacity());
    set(src);
  }

  void moveAssignFrom(DynamicJsonDocument& src) {
    freePool();
    _data = src._data;
    _pool = src._pool;
    src._data.setNull();
    src._pool = {0, 0};
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
