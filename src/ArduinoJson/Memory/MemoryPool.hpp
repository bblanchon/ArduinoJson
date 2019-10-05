// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/StringSlot.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/mpl/max.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

namespace ARDUINOJSON_NAMESPACE {

// _begin                                _end
// v                                        v
// +-------------+--------------+-----------+
// | strings...  |   (free)     |  ...slots |
// +-------------+--------------+-----------+
//               ^              ^
//             _left          _right

class MemoryPool {
 public:
  MemoryPool(char* buf, size_t capa)
      : _begin(buf),
        _left(buf),
        _right(buf ? buf + capa : 0),
        _end(buf ? buf + capa : 0) {
    ARDUINOJSON_ASSERT(isAligned(_begin));
    ARDUINOJSON_ASSERT(isAligned(_right));
    ARDUINOJSON_ASSERT(isAligned(_end));
  }

  void* buffer() {
    return _begin;
  }

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return size_t(_end - _begin);
  }

  size_t size() const {
    return size_t(_left - _begin + _end - _right);
  }

  VariantSlot* allocVariant() {
    return allocRight<VariantSlot>();
  }

  char* allocFrozenString(size_t n) {
    if (!canAlloc(n)) return 0;
    char* s = _left;
    _left += n;
    checkInvariants();
    return s;
  }

  StringSlot allocExpandableString() {
    StringSlot s;
    s.value = _left;
    s.size = size_t(_right - _left);
    _left = _right;
    checkInvariants();
    return s;
  }

  void freezeString(StringSlot& s, size_t newSize) {
    _left -= (s.size - newSize);
    s.size = newSize;
    checkInvariants();
  }

  void clear() {
    _left = _begin;
    _right = _end;
  }

  bool canAlloc(size_t bytes) const {
    return _left + bytes <= _right;
  }

  bool owns(void* p) const {
    return _begin <= p && p < _end;
  }

  template <typename T>
  T* allocRight() {
    return reinterpret_cast<T*>(allocRight(sizeof(T)));
  }

  void* allocRight(size_t bytes) {
    if (!canAlloc(bytes)) return 0;
    _right -= bytes;
    return _right;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

 private:
  StringSlot* allocStringSlot() {
    return allocRight<StringSlot>();
  }

  void checkInvariants() {
    ARDUINOJSON_ASSERT(_begin <= _left);
    ARDUINOJSON_ASSERT(_left <= _right);
    ARDUINOJSON_ASSERT(_right <= _end);
    ARDUINOJSON_ASSERT(isAligned(_right));
  }

  char *_begin, *_left, *_right, *_end;
};

}  // namespace ARDUINOJSON_NAMESPACE
