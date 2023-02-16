// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <>
class Writer<::String, void> {
  static const size_t bufferCapacity = ARDUINOJSON_STRING_BUFFER_SIZE;

 public:
  explicit Writer(::String& str) : _destination(&str) {
    _size = 0;
  }

  ~Writer() {
    flush();
  }

  size_t write(uint8_t c) {
    if (_size + 1 >= bufferCapacity)
      if (flush() != 0)
        return 0;
    _buffer[_size++] = static_cast<char>(c);
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    for (size_t i = 0; i < n; i++) {
      write(s[i]);
    }
    return n;
  }

  size_t flush() {
    ARDUINOJSON_ASSERT(_size < bufferCapacity);
    _buffer[_size] = 0;
    if (_destination->concat(_buffer))
      _size = 0;
    return _size;
  }

 private:
  ::String* _destination;
  char _buffer[bufferCapacity];
  size_t _size;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
