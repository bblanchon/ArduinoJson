// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/JsonDocument.hpp>

namespace ARDUINOJSON_NAMESPACE {

// Helper to implement the "base-from-member" idiom
// (we need to store the allocator before constructing JsonDocument)
template <typename TAllocator>
class AllocatorOwner {
 public:
  AllocatorOwner() {}
  AllocatorOwner(TAllocator a) : _allocator(a) {}

  void* allocate(size_t size) {
    return _allocator.allocate(size);
  }

  void deallocate(void* ptr) {
    if (ptr)
      _allocator.deallocate(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return _allocator.reallocate(ptr, new_size);
  }

  TAllocator& allocator() {
    return _allocator;
  }

 private:
  TAllocator _allocator;
};

// A JsonDocument that uses the provided allocator to allocate its memory pool.
// https://arduinojson.org/v6/api/basicjsondocument/
template <typename TAllocator>
class BasicJsonDocument : AllocatorOwner<TAllocator>, public JsonDocument {
 public:
  explicit BasicJsonDocument(size_t capa, TAllocator alloc = TAllocator())
      : AllocatorOwner<TAllocator>(alloc), JsonDocument(allocPool(capa)) {}

  // Copy-constructor
  BasicJsonDocument(const BasicJsonDocument& src)
      : AllocatorOwner<TAllocator>(src), JsonDocument() {
    copyAssignFrom(src);
  }

  // Move-constructor
#if ARDUINOJSON_HAS_RVALUE_REFERENCES
  BasicJsonDocument(BasicJsonDocument&& src) : AllocatorOwner<TAllocator>(src) {
    moveAssignFrom(src);
  }
#endif

  BasicJsonDocument(const JsonDocument& src) {
    copyAssignFrom(src);
  }

  // Construct from variant, array, or object
  template <typename T>
  BasicJsonDocument(
      const T& src,
      typename enable_if<
          is_same<T, JsonVariant>::value ||
          is_same<T, JsonVariantConst>::value || is_same<T, JsonArray>::value ||
          is_same<T, JsonArrayConst>::value || is_same<T, JsonObject>::value ||
          is_same<T, JsonObjectConst>::value>::type* = 0)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  // disambiguate
  BasicJsonDocument(JsonVariant src)
      : JsonDocument(allocPool(src.memoryUsage())) {
    set(src);
  }

  ~BasicJsonDocument() {
    freePool();
  }

  BasicJsonDocument& operator=(const BasicJsonDocument& src) {
    copyAssignFrom(src);
    return *this;
  }

#if ARDUINOJSON_HAS_RVALUE_REFERENCES
  BasicJsonDocument& operator=(BasicJsonDocument&& src) {
    moveAssignFrom(src);
    return *this;
  }
#endif

  template <typename T>
  BasicJsonDocument& operator=(const T& src) {
    size_t requiredSize = src.memoryUsage();
    if (requiredSize > capacity())
      reallocPool(requiredSize);
    set(src);
    return *this;
  }

  // Reduces the capacity of the memory pool to match the current usage.
  // https://arduinojson.org/v6/api/basicjsondocument/shrinktofit/
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
    BasicJsonDocument tmp(*this);
    if (!tmp.capacity())
      return false;
    tmp.set(*this);
    moveAssignFrom(tmp);
    return true;
  }

  using AllocatorOwner<TAllocator>::allocator;

 private:
  MemoryPool allocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
    return MemoryPool(reinterpret_cast<char*>(this->allocate(capa)), capa);
  }

  void reallocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
    if (capa == _pool.capacity())
      return;
    freePool();
    replacePool(allocPool(addPadding(requiredSize)));
  }

  void freePool() {
    this->deallocate(getPool()->buffer());
  }

  void copyAssignFrom(const JsonDocument& src) {
    reallocPool(src.capacity());
    set(src);
  }

  void moveAssignFrom(BasicJsonDocument& src) {
    freePool();
    _data = src._data;
    _pool = src._pool;
    src._data.setNull();
    src._pool = MemoryPool(0, 0);
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
