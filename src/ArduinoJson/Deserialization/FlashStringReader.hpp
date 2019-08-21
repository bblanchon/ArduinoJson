// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

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
