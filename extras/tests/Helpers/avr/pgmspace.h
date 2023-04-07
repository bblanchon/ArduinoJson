// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <stdint.h>  // uint8_t

#define PROGMEM

class __FlashStringHelper;

inline const void* convertPtrToFlash(const void* s) {
  return reinterpret_cast<const char*>(s) + 42;
}

inline const void* convertFlashToPtr(const void* s) {
  return reinterpret_cast<const char*>(s) - 42;
}

#define PSTR(X) reinterpret_cast<const char*>(convertPtrToFlash(X))
#define F(X) reinterpret_cast<const __FlashStringHelper*>(PSTR(X))

inline uint8_t pgm_read_byte(const void* p) {
  return *reinterpret_cast<const uint8_t*>(convertFlashToPtr(p));
}

#define ARDUINOJSON_DEFINE_PROGMEM_ARRAY(type, name, ...)                \
  static type const ARDUINOJSON_CONCAT2(name, _progmem)[] = __VA_ARGS__; \
  static type const* name = reinterpret_cast<type const*>(               \
      convertPtrToFlash(ARDUINOJSON_CONCAT2(name, _progmem)));
