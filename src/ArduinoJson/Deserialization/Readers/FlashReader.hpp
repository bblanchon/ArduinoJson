// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <>
struct Reader<const __FlashStringHelper*, void> {
  const char* _ptr;

 public:
  explicit Reader(const __FlashStringHelper* ptr)
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

template <>
struct BoundedReader<const __FlashStringHelper*, void> {
  const char* _ptr;
  const char* _end;

 public:
  explicit BoundedReader(const __FlashStringHelper* ptr, size_t size)
      : _ptr(reinterpret_cast<const char*>(ptr)), _end(_ptr + size) {}

  int read() {
    if (_ptr < _end)
      return pgm_read_byte(_ptr++);
    else
      return -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    size_t available = static_cast<size_t>(_end - _ptr);
    if (available < length)
      length = available;
    memcpy_P(buffer, _ptr, length);
    _ptr += length;
    return length;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
