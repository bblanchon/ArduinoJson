// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

class FlashStringIterator {
 public:
  explicit FlashStringIterator(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}

  explicit FlashStringIterator(const char* ptr) : _ptr(ptr) {}

  FlashStringIterator operator+(ptrdiff_t d) const {
    return FlashStringIterator(_ptr + d);
  }

  ptrdiff_t operator-(FlashStringIterator other) const {
    return _ptr - other._ptr;
  }

  FlashStringIterator operator++(int) {
    return FlashStringIterator(_ptr++);
  }

  FlashStringIterator operator++() {
    return FlashStringIterator(++_ptr);
  }

  bool operator!=(FlashStringIterator other) const {
    return _ptr != other._ptr;
  }

  char operator*() const {
    return char(pgm_read_byte(_ptr));
  }

 private:
  const char* _ptr;
};

}  // namespace ARDUINOJSON_NAMESPACE
