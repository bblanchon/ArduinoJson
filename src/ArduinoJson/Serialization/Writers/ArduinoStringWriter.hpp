// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <Arduino.h>

namespace ARDUINOJSON_NAMESPACE {

template <>
class Writer< ::String, void> {
  static const size_t bufferCapacity = ARDUINOJSON_STRING_BUFFER_SIZE;

 public:
  explicit Writer(::String &str) : _destination(&str) {
    _size = 0;
  }

  ~Writer() {
    flush();
  }

  size_t write(uint8_t c) {
    ARDUINOJSON_ASSERT(_size < bufferCapacity);
    _buffer[_size++] = static_cast<char>(c);
    if (_size + 1 >= bufferCapacity)
      flush();
    return 1;
  }

  size_t write(const uint8_t *s, size_t n) {
    for (size_t i = 0; i < n; i++) {
      write(s[i]);
    }
    return n;
  }

 private:
  void flush() {
    ARDUINOJSON_ASSERT(_size < bufferCapacity);
    _buffer[_size] = 0;
    *_destination += _buffer;
    _size = 0;
  }

  ::String *_destination;
  char _buffer[bufferCapacity];
  size_t _size;
};

}  // namespace ARDUINOJSON_NAMESPACE
