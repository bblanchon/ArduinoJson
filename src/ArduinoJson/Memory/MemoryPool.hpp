// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t
#include <string.h>

#include "../Configuration.hpp"
#include "../Data/Slot.hpp"
#include "../Polyfills/attributes.hpp"

namespace ARDUINOJSON_NAMESPACE {
// Handle the memory management (done in derived classes) and calls the parser.
// This abstract class is implemented by StaticMemoryPool which implements a
// fixed memory allocation.
class MemoryPool {
 public:
  // Allocates n bytes in the MemoryPool.
  // Return a pointer to the allocated memory or NULL if allocation fails.
  virtual char *alloc(size_t size) = 0;

  virtual char *realloc(char *oldPtr, size_t oldSize, size_t newSize) = 0;

  Slot *allocSlot() {
    if (_freeSlots) {
      Slot *s = _freeSlots;
      _freeSlots = s->next;
      return s;
    }
    return reinterpret_cast<Slot *>(alloc(sizeof(Slot)));
  }

  void freeSlot(Slot *slot) {
    slot->next = _freeSlots;
    _freeSlots = slot;
  }

  size_t size() const {
    size_t result = allocated_bytes();
    for (Slot *s = _freeSlots; s; s = s->next) result -= sizeof(Slot);
    return result;
  }

 protected:
  MemoryPool() : _freeSlots(0) {}

  // CAUTION: NO VIRTUAL DESTRUCTOR!
  // If we add a virtual constructor the Arduino compiler will add malloc()
  // and free() to the binary, adding 706 useless bytes.
  ~MemoryPool() {}

  virtual size_t allocated_bytes() const = 0;

  // Preserve aligment if necessary
  static FORCE_INLINE size_t round_size_up(size_t bytes) {
#if ARDUINOJSON_ENABLE_ALIGNMENT
    const size_t x = sizeof(void *) - 1;
    return (bytes + x) & ~x;
#else
    return bytes;
#endif
  }

 private:
  Slot *_freeSlots;
};
}  // namespace ARDUINOJSON_NAMESPACE
