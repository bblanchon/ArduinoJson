// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/mpl/max.hpp"
#include "MemoryPool.hpp"

namespace ArduinoJson {
namespace Internals {

class StaticMemoryPoolBase : public MemoryPool {
 public:
  class String {
   public:
    String(StaticMemoryPoolBase* parent) : _parent(parent) {
      _start = parent->_buffer + parent->_size;
    }

    void append(char c) {
      if (_parent->canAlloc(1)) {
        char* last = static_cast<char*>(_parent->doAlloc(1));
        *last = c;
      }
    }

    const char* c_str() const {
      if (_parent->canAlloc(1)) {
        char* last = static_cast<char*>(_parent->doAlloc(1));
        *last = '\0';
        return _start;
      } else {
        return NULL;
      }
    }

   private:
    StaticMemoryPoolBase* _parent;
    char* _start;
  };

  StaticMemoryPoolBase(char* memoryPool, size_t capa)
      : _buffer(memoryPool), _capacity(capa), _size(0) {}

  // Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return _capacity;
  }

  // Gets the current usage of the memoryPool in bytes
  size_t size() const {
    return _size;
  }

  // Allocates the specified amount of bytes in the memoryPool
  virtual void* alloc(size_t bytes) {
    alignNextAlloc();
    if (!canAlloc(bytes)) return NULL;
    return doAlloc(bytes);
  }

  // Resets the memoryPool.
  // USE WITH CAUTION: this invalidates all previously allocated data
  void clear() {
    _size = 0;
  }

  String startString() {
    return String(this);
  }

 protected:
  ~StaticMemoryPoolBase() {}

 private:
  void alignNextAlloc() {
    _size = round_size_up(_size);
  }

  bool canAlloc(size_t bytes) const {
    return _size + bytes <= _capacity;
  }

  void* doAlloc(size_t bytes) {
    void* p = &_buffer[_size];
    _size += bytes;
    return p;
  }

  char* _buffer;
  size_t _capacity;
  size_t _size;
};

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

// Implements a MemoryPool with fixed memory allocation.
// The template paramenter CAPACITY specifies the capacity of the memoryPool in
// bytes.
template <size_t CAPACITY>
class StaticMemoryPool : public StaticMemoryPoolBase {
  static const size_t ACTUAL_CAPACITY = Max<1, CAPACITY>::value;

 public:
  explicit StaticMemoryPool()
      : StaticMemoryPoolBase(_buffer, ACTUAL_CAPACITY) {}

 private:
  char _buffer[ACTUAL_CAPACITY];
};
}  // namespace Internals
}  // namespace ArduinoJson

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
