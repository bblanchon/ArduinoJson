// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <>
class Writer<::String, void> {
  static const size_t bufferCapacity = ARDUINOJSON_STRING_BUFFER_SIZE;

 public:
  explicit Writer(::String& str) : destination_(&str), size_(0) {
    // clear the string but don't use "" to avoid useless allocation
    // https://cpp4arduino.com/2018/11/21/eight-tips-to-use-the-string-class-efficiently.html
    str = static_cast<const char*>(0);
  }

  ~Writer() {
    flush();
  }

  size_t write(uint8_t c) {
    if (size_ + 1 >= bufferCapacity)
      if (flush() != 0)
        return 0;
    buffer_[size_++] = static_cast<char>(c);
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    for (size_t i = 0; i < n; i++) {
      write(s[i]);
    }
    return n;
  }

  size_t flush() {
    ARDUINOJSON_ASSERT(size_ < bufferCapacity);
    buffer_[size_] = 0;
    if (destination_->concat(buffer_))
      size_ = 0;
    return size_;
  }

 private:
  ::String* destination_;
  char buffer_[bufferCapacity];
  size_t size_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
