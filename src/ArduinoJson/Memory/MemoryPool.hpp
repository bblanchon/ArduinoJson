// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/Allocator.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/mpl/max.hpp>
#include <ArduinoJson/Strings/StringAdapters.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

#include <string.h>  // memmove

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// Returns the size (in bytes) of an array with n elements.
constexpr size_t sizeofArray(size_t n) {
  return n * sizeof(VariantSlot);
}

// Returns the size (in bytes) of an object with n members.
constexpr size_t sizeofObject(size_t n) {
  return n * sizeof(VariantSlot);
}

struct StringNode {
  struct StringNode* next;
  uint16_t length;
  uint16_t references;
  char data[1];
};

// Returns the size (in bytes) of an string with n characters.
constexpr size_t sizeofString(size_t n) {
  return n + 1 + offsetof(StringNode, data);
}

class MemoryPool {
 public:
  MemoryPool(size_t capa, Allocator* allocator = DefaultAllocator::instance())
      : _allocator(allocator), _overflowed(false) {
    allocPool(addPadding(capa));
  }

  ~MemoryPool() {
    deallocAllStrings();
    deallocPool();
  }

  MemoryPool(const MemoryPool&) = delete;
  MemoryPool& operator=(const MemoryPool& src) = delete;

  MemoryPool& operator=(MemoryPool&& src) {
    deallocAllStrings();
    deallocPool();
    _allocator = src._allocator;
    _begin = src._begin;
    _end = src._end;
    _right = src._right;
    _overflowed = src._overflowed;
    src._begin = src._end = src._right = nullptr;
    _strings = src._strings;
    src._strings = nullptr;
    return *this;
  }

  Allocator* allocator() const {
    return _allocator;
  }

  void reallocPool(size_t requiredSize) {
    size_t capa = addPadding(requiredSize);
    if (capa == capacity())
      return;
    _allocator->deallocate(_begin);
    allocPool(requiredSize);
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
    size_t total = size_t(_end - _right);
    for (auto node = _strings; node; node = node->next)
      total += sizeofString(node->length);
    return total;
  }

  bool overflowed() const {
    return _overflowed;
  }

  VariantSlot* allocVariant() {
    auto slot = allocRight<VariantSlot>();
    if (slot)
      slot->clear();
    return slot;
  }

  template <typename TAdaptedString>
  const char* saveString(TAdaptedString str) {
    if (str.isNull())
      return 0;

    auto node = findString(str);
    if (node) {
      node->references++;
      return node->data;
    }

    size_t n = str.size();

    node = allocString(n);
    if (!node)
      return nullptr;

    stringGetChars(str, node->data, n);
    node->data[n] = 0;  // force NUL terminator
    addStringToList(node);
    return node->data;
  }

  void addStringToList(StringNode* node) {
    ARDUINOJSON_ASSERT(node != nullptr);
    node->next = _strings;
    _strings = node;
  }

  template <typename TAdaptedString>
  StringNode* findString(const TAdaptedString& str) const {
    for (auto node = _strings; node; node = node->next) {
      if (stringEquals(str, adaptString(node->data, node->length)))
        return node;
    }
    return nullptr;
  }

  StringNode* allocString(size_t length) {
    auto node = reinterpret_cast<StringNode*>(
        _allocator->allocate(sizeofString(length)));
    if (node) {
      node->length = uint16_t(length);
      node->references = 1;
    } else {
      _overflowed = true;
    }
    return node;
  }

  StringNode* reallocString(StringNode* node, size_t length) {
    ARDUINOJSON_ASSERT(node != nullptr);
    auto newNode = reinterpret_cast<StringNode*>(
        _allocator->reallocate(node, sizeofString(length)));
    if (newNode) {
      newNode->length = uint16_t(length);
    } else {
      _overflowed = true;
      _allocator->deallocate(node);
    }
    return newNode;
  }

  void deallocString(StringNode* node) {
    _allocator->deallocate(node);
  }

  void dereferenceString(const char* s) {
    StringNode* prev = nullptr;
    for (auto node = _strings; node; node = node->next) {
      if (node->data == s) {
        if (--node->references == 0) {
          if (prev)
            prev->next = node->next;
          else
            _strings = node->next;
          _allocator->deallocate(node);
        }
        return;
      }
      prev = node;
    }
  }

  void clear() {
    _right = _end;
    _overflowed = false;
    deallocAllStrings();
  }

  bool canAlloc(size_t bytes) const {
    return _begin + bytes <= _right;
  }

  bool owns(void* p) const {
    return _begin <= p && p < _end;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

  void shrinkToFit(VariantData& variant) {
    ptrdiff_t bytes_reclaimed = squash();
    if (bytes_reclaimed == 0)
      return;

    void* old_ptr = _begin;
    void* new_ptr = _allocator->reallocate(old_ptr, capacity());

    ptrdiff_t ptr_offset =
        static_cast<char*>(new_ptr) - static_cast<char*>(old_ptr);

    movePointers(ptr_offset);
    reinterpret_cast<VariantSlot&>(variant).movePointers(ptr_offset -
                                                         bytes_reclaimed);
  }

 private:
  ptrdiff_t squash() {
    char* new_right = addPadding(_begin);
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
    _right += offset;
    _end += offset;
  }

  void checkInvariants() {
    ARDUINOJSON_ASSERT(_begin <= _right);
    ARDUINOJSON_ASSERT(_right <= _end);
    ARDUINOJSON_ASSERT(isAligned(_right));
  }

  void deallocAllStrings() {
    while (_strings) {
      auto node = _strings;
      _strings = node->next;
      deallocString(node);
    }
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

  void allocPool(size_t capa) {
    auto buf = capa ? reinterpret_cast<char*>(_allocator->allocate(capa)) : 0;
    _begin = buf;
    _end = _right = buf ? buf + capa : 0;
    ARDUINOJSON_ASSERT(isAligned(_begin));
    ARDUINOJSON_ASSERT(isAligned(_right));
    ARDUINOJSON_ASSERT(isAligned(_end));
  }

  void deallocPool() {
    if (_begin)
      _allocator->deallocate(_begin);
  }

  Allocator* _allocator;
  char *_begin, *_right, *_end;
  bool _overflowed;
  StringNode* _strings = nullptr;
};

template <typename TAdaptedString>
JsonString storeString(MemoryPool* pool, TAdaptedString str,
                       StringStoragePolicy::Copy) {
  return JsonString(pool->saveString(str), str.size(), JsonString::Copied);
}

template <typename TAdaptedString>
JsonString storeString(MemoryPool*, TAdaptedString str,
                       StringStoragePolicy::Link) {
  return JsonString(str.data(), str.size(), JsonString::Linked);
}

template <typename TAdaptedString>
JsonString storeString(MemoryPool* pool, TAdaptedString str,
                       StringStoragePolicy::LinkOrCopy policy) {
  if (policy.link)
    return storeString(pool, str, StringStoragePolicy::Link());
  else
    return storeString(pool, str, StringStoragePolicy::Copy());
}

template <typename TAdaptedString>
JsonString storeString(MemoryPool* pool, TAdaptedString str) {
  return storeString(pool, str, str.storagePolicy());
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
