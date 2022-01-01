// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/mpl/max.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

#include <string.h>  // memmove

#define JSON_STRING_SIZE(SIZE) (SIZE + 1)

namespace ARDUINOJSON_NAMESPACE {

// _begin                                   _end
// v                                           v
// +-------------+--------------+--------------+
// | strings...  |   (free)     |  ...variants |
// +-------------+--------------+--------------+
//               ^              ^
//             _left          _right

class MemoryPool {
 public:
  MemoryPool(char* buf, size_t capa)
      : _begin(buf),
        _left(buf),
        _right(buf ? buf + capa : 0),
        _end(buf ? buf + capa : 0),
        _overflowed(false) {
    ARDUINOJSON_ASSERT(isAligned(_begin));
    ARDUINOJSON_ASSERT(isAligned(_right));
    ARDUINOJSON_ASSERT(isAligned(_end));
  }

  void* buffer() {
    return _begin;  // NOLINT(clang-analyzer-unix.Malloc)
                    // movePointers() alters this pointer
  }

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return size_t(_end - _begin);
  }

  size_t size() const {
    return size_t(_left - _begin + _end - _right);
  }

  bool overflowed() const {
    return _overflowed;
  }

  VariantSlot* allocVariant() {
    return allocRight<VariantSlot>();
  }

  template <typename TAdaptedString>
  const char* saveString(TAdaptedString str) {
    if (str.isNull())
      return CopiedString();

#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
    const char* existingCopy = findString(str);
    if (existingCopy)
      return CopiedString(existingCopy, str.size());
#endif

    size_t n = str.size();

    char* newCopy = allocString(n + 1);
    if (newCopy) {
      stringGetChars(str, newCopy, n);
      newCopy[n] = 0;  // force null-terminator
    }
    return CopiedString(newCopy, n);
  }

  void getFreeZone(char** zoneStart, size_t* zoneSize) const {
    *zoneStart = _left;
    *zoneSize = size_t(_right - _left);
  }

  const char* saveStringFromFreeZone(size_t len) {
#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
    const char* dup = findString(adaptString(_left, len));
    if (dup)
      return CopiedString(dup, len);
#endif

    const char* str = _left;
    _left += len;
    *_left++ = 0;
    checkInvariants();
    return CopiedString(str, len);
  }

  void markAsOverflowed() {
    _overflowed = true;
  }

  void clear() {
    _left = _begin;
    _right = _end;
    _overflowed = false;
  }

  bool canAlloc(size_t bytes) const {
    return _left + bytes <= _right;
  }

  bool owns(void* p) const {
    return _begin <= p && p < _end;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

  // Squash the free space between strings and variants
  //
  // _begin                    _end
  // v                            v
  // +-------------+--------------+
  // | strings...  |  ...variants |
  // +-------------+--------------+
  //               ^
  //          _left _right
  //
  // This funcion is called before a realloc.
  ptrdiff_t squash() {
    char* new_right = addPadding(_left);
    if (new_right >= _right)
      return 0;

    size_t right_size = static_cast<size_t>(_end - _right);
    memmove(new_right, _right, right_size);

    ptrdiff_t bytes_reclaimed = _right - new_right;
    _right = new_right;
    _end = new_right + right_size;
    return bytes_reclaimed;
  }

  // Move all pointers together
  // This funcion is called after a realloc.
  void movePointers(ptrdiff_t offset) {
    _begin += offset;
    _left += offset;
    _right += offset;
    _end += offset;
  }

 private:
  void checkInvariants() {
    ARDUINOJSON_ASSERT(_begin <= _left);
    ARDUINOJSON_ASSERT(_left <= _right);
    ARDUINOJSON_ASSERT(_right <= _end);
    ARDUINOJSON_ASSERT(isAligned(_right));
  }

#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
  template <typename TAdaptedString>
  const char* findString(const TAdaptedString& str) const {
    size_t n = str.size();
    for (char* next = _begin; next + n < _left; ++next) {
      if (next[n] == '\0' && stringEquals(str, adaptString(next, n)))
        return next;

      // jump to next terminator
      while (*next) ++next;
    }
    return 0;
  }
#endif

  char* allocString(size_t n) {
    if (!canAlloc(n)) {
      _overflowed = true;
      return 0;
    }
    char* s = _left;
    _left += n;
    checkInvariants();
    return s;
  }

  template <typename T>
  T* allocRight() {
    return reinterpret_cast<T*>(allocRight(sizeof(T)));
  }

  void* allocRight(size_t bytes) {
    if (!canAlloc(bytes)) {
      _overflowed = true;
      return 0;
    }
    _right -= bytes;
    return _right;
  }

  char *_begin, *_left, *_right, *_end;
  bool _overflowed;
};

}  // namespace ARDUINOJSON_NAMESPACE
