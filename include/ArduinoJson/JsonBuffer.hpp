// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t
#include <string.h>

#include "Arduino/String.hpp"
#include "JsonVariant.hpp"

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

namespace ArduinoJson {
class JsonArray;
class JsonObject;

// Entry point for using the library.
//
// Handle the memory management (done in derived classes) and calls the parser.
// This abstract class is implemented by StaticJsonBuffer which implements a
// fixed memory allocation.
class JsonBuffer {
 public:
  // CAUTION: NO VIRTUAL DESTRUCTOR!
  // If we add a virtual constructor the Arduino compiler will add malloc() and
  // free() to the binary, adding 706 useless bytes.
  // virtual ~JsonBuffer() {}

  // Allocates an empty JsonArray.
  //
  // Returns a reference to the new JsonArray or JsonArray::invalid() if the
  // allocation fails.
  JsonArray &createArray();

  // Allocates an empty JsonObject.
  //
  // Returns a reference to the new JsonObject or JsonObject::invalid() if the
  // allocation fails.
  JsonObject &createObject();

  // Allocates and populate a JsonArray from a JSON string.
  //
  // The First argument is a pointer to the JSON string, the memory must be
  // writable
  // because the parser will insert null-terminators and replace escaped chars.
  //
  // The second argument set the nesting limit (see comment on DEFAULT_LIMIT)
  //
  // Returns a reference to the new JsonObject or JsonObject::invalid() if the
  // allocation fails.
  JsonArray &parseArray(char *json, uint8_t nestingLimit = DEFAULT_LIMIT);

  // Same with a const char*.
  // With this overload, the JsonBuffer will make a copy of the string
  JsonArray &parseArray(const char *json, uint8_t nesting = DEFAULT_LIMIT) {
    return parseArray(strdup(json), nesting);
  }

  // Same as above with a String class
  JsonArray &parseArray(const String &json, uint8_t nesting = DEFAULT_LIMIT) {
    return parseArray(json.c_str(), nesting);
  }

  // Allocates and populate a JsonObject from a JSON string.
  //
  // The First argument is a pointer to the JSON string, the memory must be
  // writable
  // because the parser will insert null-terminators and replace escaped chars.
  //
  // The second argument set the nesting limit (see comment on DEFAULT_LIMIT)
  //
  // Returns a reference to the new JsonObject or JsonObject::invalid() if the
  // allocation fails.
  JsonObject &parseObject(char *json, uint8_t nestingLimit = DEFAULT_LIMIT);

  // Same with a const char*.
  // With this overload, the JsonBuffer will make a copy of the string
  JsonObject &parseObject(const char *json, uint8_t nesting = DEFAULT_LIMIT) {
    return parseObject(strdup(json), nesting);
  }

  // Same as above with a String class
  JsonObject &parseObject(const String &json, uint8_t nesting = DEFAULT_LIMIT) {
    return parseObject(json.c_str(), nesting);
  }

  // Duplicate a string
  char *strdup(const char *src) {
    return src ? strdup(src, strlen(src)) : NULL;
  }
  char *strdup(const String &src) { return strdup(src.c_str(), src.length()); }

  // Allocates n bytes in the JsonBuffer.
  // Return a pointer to the allocated memory or NULL if allocation fails.
  virtual void *alloc(size_t size) = 0;

 protected:
  // Preserve aligment if nessary
  static FORCE_INLINE size_t round_size_up(size_t bytes) {
#if defined ARDUINO_ARCH_AVR
    // alignment isn't needed for 8-bit AVR
    return bytes;
#else
    const size_t x = sizeof(void *) - 1;
    return (bytes + x) & ~x;
#endif
  }

 private:
  char *strdup(const char *, size_t);

  // Default value of nesting limit of parseArray() and parseObject().
  //
  // The nesting limit is a contain on the level of nesting allowed in the
  // JSON
  // string.
  // If set to 0, only a flat array or objects can be parsed.
  // If set to 1, the object can contain nested arrays or objects but only 1
  // level deep.
  // And bigger values will allow more level of nesting.
  //
  // The purpose of this feature is to prevent stack overflow that could
  // lead to
  // a security risk.
  static const uint8_t DEFAULT_LIMIT = 10;
};
}
