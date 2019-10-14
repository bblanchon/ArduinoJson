// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#if ARDUINOJSON_ENABLE_PROGMEM

namespace ARDUINOJSON_NAMESPACE {
class UnsafeFlashStringReader {
  const char* _ptr;

 public:
  explicit UnsafeFlashStringReader(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}

  int read() {
    return pgm_read_byte(_ptr++);
  }

  size_t readBytes(char* buffer, size_t length) {
    memcpy_P(buffer, _ptr, length);
    _ptr += length;
    return length;
  }
};

class SafeFlashStringReader {
  const char* _ptr;
  const char* _end;

 public:
  explicit SafeFlashStringReader(const __FlashStringHelper* ptr, size_t size)
      : _ptr(reinterpret_cast<const char*>(ptr)), _end(_ptr + size) {}

  int read() {
    if (_ptr < _end)
      return pgm_read_byte(_ptr++);
    else
      return -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    size_t available = static_cast<size_t>(_end - _ptr);
    if (available < length) length = available;
    memcpy_P(buffer, _ptr, length);
    _ptr += length;
    return length;
  }
};

inline UnsafeFlashStringReader makeReader(const __FlashStringHelper* input) {
  return UnsafeFlashStringReader(input);
}

inline SafeFlashStringReader makeReader(const __FlashStringHelper* input,
                                        size_t size) {
  return SafeFlashStringReader(input, size);
}
}  // namespace ARDUINOJSON_NAMESPACE

#endif
