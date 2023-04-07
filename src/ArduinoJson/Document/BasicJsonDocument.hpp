// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Document/JsonDocument.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// Helper to implement the "base-from-member" idiom
// (we need to store the allocator before constructing JsonDocument)
template <typename TAllocator>
class AllocatorOwner {
 public:
  AllocatorOwner() {}
  AllocatorOwner(TAllocator a) : allocator_(a) {}

  void* allocate(size_t size) {
    return allocator_.allocate(size);
  }

  void deallocate(void* ptr) {
    if (ptr)
      allocator_.deallocate(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return allocator_.reallocate(ptr, new_size);
  }

  TAllocator& allocator() {
    return allocator_;
  }

 private:
  TAllocator allocator_;
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
  BasicJsonDocument(BasicJsonDocument&& src) : AllocatorOwner<TAllocator>(src) {
    moveAssignFrom(src);
  }

  BasicJsonDocument(const JsonDocument& src) {
    copyAssignFrom(src);
  }

  // Construct from variant, array, or object
  template <typename T>
  BasicJsonDocument(const T& src,
                    typename detail::enable_if<
                        detail::is_same<T, JsonVariant>::value ||
                        detail::is_same<T, JsonVariantConst>::value ||
                        detail::is_same<T, JsonArray>::value ||
                        detail::is_same<T, JsonArrayConst>::value ||
                        detail::is_same<T, JsonObject>::value ||
                        detail::is_same<T, JsonObjectConst>::value>::type* = 0)
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

  BasicJsonDocument& operator=(BasicJsonDocument&& src) {
    moveAssignFrom(src);
    return *this;
  }

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
    ptrdiff_t bytes_reclaimed = pool_.squash();
    if (bytes_reclaimed == 0)
      return;

    void* old_ptr = pool_.buffer();
    void* new_ptr = this->reallocate(old_ptr, pool_.capacity());

    ptrdiff_t ptr_offset =
        static_cast<char*>(new_ptr) - static_cast<char*>(old_ptr);

    pool_.movePointers(ptr_offset);
    data_.movePointers(ptr_offset, ptr_offset - bytes_reclaimed);
  }

  // Reclaims the memory leaked when removing and replacing values.
  // https://arduinojson.org/v6/api/jsondocument/garbagecollect/
  bool garbageCollect() {
    // make a temporary clone and move assign
    BasicJsonDocument tmp(*this);
    if (!tmp.capacity())
      return false;
    moveAssignFrom(tmp);
    return true;
  }

  using AllocatorOwner<TAllocator>::allocator;

 private:
  detail::MemoryPool allocPool(size_t requiredSize) {
    size_t capa = detail::addPadding(requiredSize);
    return {reinterpret_cast<char*>(this->allocate(capa)), capa};
  }

  void reallocPool(size_t requiredSize) {
    size_t capa = detail::addPadding(requiredSize);
    if (capa == pool_.capacity())
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

  void moveAssignFrom(BasicJsonDocument& src) {
    freePool();
    data_ = src.data_;
    pool_ = src.pool_;
    src.data_.setNull();
    src.pool_ = {0, 0};
  }
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
