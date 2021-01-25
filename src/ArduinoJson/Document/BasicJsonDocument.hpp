// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
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
  AllocatorOwner(const AllocatorOwner& src) : _allocator(src._allocator) {}
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
          is_same<T, VariantRef>::value || is_same<T, VariantConstRef>::value ||
          is_same<T, ArrayRef>::value || is_same<T, ArrayConstRef>::value ||
          is_same<T, ObjectRef>::value ||
          is_same<T, ObjectConstRef>::value>::type* = 0)
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
    reallocPoolIfTooSmall(src.memoryUsage());
    set(src);
    return *this;
  }

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

  void reallocPoolIfTooSmall(size_t requiredSize) {
    if (requiredSize <= capacity())
      return;
    freePool();
    replacePool(allocPool(addPadding(requiredSize)));
  }

  void freePool() {
    this->deallocate(memoryPool().buffer());
  }

  void copyAssignFrom(const JsonDocument& src) {
    reallocPoolIfTooSmall(src.capacity());
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
