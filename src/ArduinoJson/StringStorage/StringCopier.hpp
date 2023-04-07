// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class StringCopier {
 public:
  StringCopier(MemoryPool* pool) : pool_(pool) {}

  void startString() {
    pool_->getFreeZone(&ptr_, &capacity_);
    size_ = 0;
    if (capacity_ == 0)
      pool_->markAsOverflowed();
  }

  JsonString save() {
    ARDUINOJSON_ASSERT(ptr_);
    ARDUINOJSON_ASSERT(size_ < capacity_);  // needs room for the terminator
    return JsonString(pool_->saveStringFromFreeZone(size_), size_,
                      JsonString::Copied);
  }

  void append(const char* s) {
    while (*s)
      append(*s++);
  }

  void append(const char* s, size_t n) {
    while (n-- > 0)
      append(*s++);
  }

  void append(char c) {
    if (size_ + 1 < capacity_)
      ptr_[size_++] = c;
    else
      pool_->markAsOverflowed();
  }

  bool isValid() const {
    return !pool_->overflowed();
  }

  size_t size() const {
    return size_;
  }

  JsonString str() const {
    ARDUINOJSON_ASSERT(ptr_);
    ARDUINOJSON_ASSERT(size_ < capacity_);
    ptr_[size_] = 0;
    return JsonString(ptr_, size_, JsonString::Copied);
  }

 private:
  MemoryPool* pool_;

  // These fields aren't initialized by the constructor but startString()
  //
  // NOLINTNEXTLINE(clang-analyzer-optin.cplusplus.UninitializedObject)
  char* ptr_;
  // NOLINTNEXTLINE(clang-analyzer-optin.cplusplus.UninitializedObject)
  size_t size_, capacity_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
