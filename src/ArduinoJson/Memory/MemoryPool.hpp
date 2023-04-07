// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/mpl/max.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

#include <string.h>  // memmove

#define JSON_STRING_SIZE(SIZE) (SIZE + 1)

// Computes the size required to store an array in a JsonDocument.
// https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ArduinoJson::detail::VariantSlot))

// Returns the size (in bytes) of an object with n elements.
// Can be very handy to determine the size of a StaticMemoryPool.
#define JSON_OBJECT_SIZE(NUMBER_OF_ELEMENTS) \
  ((NUMBER_OF_ELEMENTS) * sizeof(ArduinoJson::detail::VariantSlot))

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// begin_                                   end_
// v                                           v
// +-------------+--------------+--------------+
// | strings...  |   (free)     |  ...variants |
// +-------------+--------------+--------------+
//               ^              ^
//             left_          right_

class MemoryPool {
 public:
  MemoryPool(char* buf, size_t capa)
      : begin_(buf),
        left_(buf),
        right_(buf ? buf + capa : 0),
        end_(buf ? buf + capa : 0),
        overflowed_(false) {
    ARDUINOJSON_ASSERT(isAligned(begin_));
    ARDUINOJSON_ASSERT(isAligned(right_));
    ARDUINOJSON_ASSERT(isAligned(end_));
  }

  void* buffer() {
    return begin_;  // NOLINT(clang-analyzer-unix.Malloc)
                    // movePointers() alters this pointer
  }

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return size_t(end_ - begin_);
  }

  size_t size() const {
    return size_t(left_ - begin_ + end_ - right_);
  }

  bool overflowed() const {
    return overflowed_;
  }

  VariantSlot* allocVariant() {
    return allocRight<VariantSlot>();
  }

  template <typename TAdaptedString>
  const char* saveString(TAdaptedString str) {
    if (str.isNull())
      return 0;

#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
    const char* existingCopy = findString(str);
    if (existingCopy)
      return existingCopy;
#endif

    size_t n = str.size();

    char* newCopy = allocString(n + 1);
    if (newCopy) {
      stringGetChars(str, newCopy, n);
      newCopy[n] = 0;  // force null-terminator
    }
    return newCopy;
  }

  void getFreeZone(char** zoneStart, size_t* zoneSize) const {
    *zoneStart = left_;
    *zoneSize = size_t(right_ - left_);
  }

  const char* saveStringFromFreeZone(size_t len) {
#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
    const char* dup = findString(adaptString(left_, len));
    if (dup)
      return dup;
#endif

    const char* str = left_;
    left_ += len;
    *left_++ = 0;
    checkInvariants();
    return str;
  }

  void markAsOverflowed() {
    overflowed_ = true;
  }

  void clear() {
    left_ = begin_;
    right_ = end_;
    overflowed_ = false;
  }

  bool canAlloc(size_t bytes) const {
    return left_ + bytes <= right_;
  }

  bool owns(void* p) const {
    return begin_ <= p && p < end_;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

  // Squash the free space between strings and variants
  //
  // begin_                    end_
  // v                            v
  // +-------------+--------------+
  // | strings...  |  ...variants |
  // +-------------+--------------+
  //               ^
  //          left_ right_
  //
  // This funcion is called before a realloc.
  ptrdiff_t squash() {
    char* new_right = addPadding(left_);
    if (new_right >= right_)
      return 0;

    size_t right_size = static_cast<size_t>(end_ - right_);
    memmove(new_right, right_, right_size);

    ptrdiff_t bytes_reclaimed = right_ - new_right;
    right_ = new_right;
    end_ = new_right + right_size;
    return bytes_reclaimed;
  }

  // Move all pointers together
  // This funcion is called after a realloc.
  void movePointers(ptrdiff_t offset) {
    begin_ += offset;
    left_ += offset;
    right_ += offset;
    end_ += offset;
  }

 private:
  void checkInvariants() {
    ARDUINOJSON_ASSERT(begin_ <= left_);
    ARDUINOJSON_ASSERT(left_ <= right_);
    ARDUINOJSON_ASSERT(right_ <= end_);
    ARDUINOJSON_ASSERT(isAligned(right_));
  }

#if ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
  template <typename TAdaptedString>
  const char* findString(const TAdaptedString& str) const {
    size_t n = str.size();
    for (char* next = begin_; next + n < left_; ++next) {
      if (next[n] == '\0' && stringEquals(str, adaptString(next, n)))
        return next;

      // jump to next terminator
      while (*next)
        ++next;
    }
    return 0;
  }
#endif

  char* allocString(size_t n) {
    if (!canAlloc(n)) {
      overflowed_ = true;
      return 0;
    }
    char* s = left_;
    left_ += n;
    checkInvariants();
    return s;
  }

  template <typename T>
  T* allocRight() {
    return reinterpret_cast<T*>(allocRight(sizeof(T)));
  }

  void* allocRight(size_t bytes) {
    if (!canAlloc(bytes)) {
      overflowed_ = true;
      return 0;
    }
    right_ -= bytes;
    return right_;
  }

  char *begin_, *left_, *right_, *end_;
  bool overflowed_;
};

template <typename TAdaptedString, typename TCallback>
bool storeString(MemoryPool* pool, TAdaptedString str,
                 StringStoragePolicy::Copy, TCallback callback) {
  const char* copy = pool->saveString(str);
  JsonString storedString(copy, str.size(), JsonString::Copied);
  callback(storedString);
  return copy != 0;
}

template <typename TAdaptedString, typename TCallback>
bool storeString(MemoryPool*, TAdaptedString str, StringStoragePolicy::Link,
                 TCallback callback) {
  JsonString storedString(str.data(), str.size(), JsonString::Linked);
  callback(storedString);
  return !str.isNull();
}

template <typename TAdaptedString, typename TCallback>
bool storeString(MemoryPool* pool, TAdaptedString str,
                 StringStoragePolicy::LinkOrCopy policy, TCallback callback) {
  if (policy.link)
    return storeString(pool, str, StringStoragePolicy::Link(), callback);
  else
    return storeString(pool, str, StringStoragePolicy::Copy(), callback);
}

template <typename TAdaptedString, typename TCallback>
bool storeString(MemoryPool* pool, TAdaptedString str, TCallback callback) {
  return storeString(pool, str, str.storagePolicy(), callback);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
