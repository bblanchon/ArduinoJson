// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t
#include <string.h>

#include "../Configuration.hpp"
#include "../Polyfills/attributes.hpp"

namespace ArduinoJson {
namespace Internals {
// Handle the memory management (done in derived classes) and calls the parser.
// This abstract class is implemented by StaticMemoryPool which implements a
// fixed memory allocation.
class MemoryPool {
 public:
  // Allocates n bytes in the MemoryPool.
  // Return a pointer to the allocated memory or NULL if allocation fails.
  virtual void *alloc(size_t size) = 0;

 protected:
  // CAUTION: NO VIRTUAL DESTRUCTOR!
  // If we add a virtual constructor the Arduino compiler will add malloc()
  // and free() to the binary, adding 706 useless bytes.
  ~MemoryPool() {}

  // Preserve aligment if necessary
  static FORCE_INLINE size_t round_size_up(size_t bytes) {
#if ARDUINOJSON_ENABLE_ALIGNMENT
    const size_t x = sizeof(void *) - 1;
    return (bytes + x) & ~x;
#else
    return bytes;
#endif
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
