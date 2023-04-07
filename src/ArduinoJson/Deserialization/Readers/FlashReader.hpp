// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/pgmspace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <>
struct Reader<const __FlashStringHelper*, void> {
  const char* ptr_;

 public:
  explicit Reader(const __FlashStringHelper* ptr)
      : ptr_(reinterpret_cast<const char*>(ptr)) {}

  int read() {
    return pgm_read_byte(ptr_++);
  }

  size_t readBytes(char* buffer, size_t length) {
    memcpy_P(buffer, ptr_, length);
    ptr_ += length;
    return length;
  }
};

template <>
struct BoundedReader<const __FlashStringHelper*, void> {
  const char* ptr_;
  const char* end_;

 public:
  explicit BoundedReader(const __FlashStringHelper* ptr, size_t size)
      : ptr_(reinterpret_cast<const char*>(ptr)), end_(ptr_ + size) {}

  int read() {
    if (ptr_ < end_)
      return pgm_read_byte(ptr_++);
    else
      return -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    size_t available = static_cast<size_t>(end_ - ptr_);
    if (available < length)
      length = available;
    memcpy_P(buffer, ptr_, length);
    ptr_ += length;
    return length;
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
