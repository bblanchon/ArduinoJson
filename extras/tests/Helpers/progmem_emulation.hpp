// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <stdint.h>  // uint8_t
#include <string.h>  // strcmp, strlen...

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

inline void* pgm_read_ptr(const void* p) {
  return *reinterpret_cast<void* const*>(convertFlashToPtr(p));
}

inline float pgm_read_float(const void* p) {
  return *reinterpret_cast<const float*>(convertFlashToPtr(p));
}

inline uint32_t pgm_read_dword(const void* p) {
  return *reinterpret_cast<const uint32_t*>(convertFlashToPtr(p));
}

#define ARDUINOJSON_DEFINE_STATIC_ARRAY(type, name, value)         \
  static type const ARDUINOJSON_CONCAT2(name, _progmem)[] = value; \
  static type const* name = reinterpret_cast<type const*>(         \
      convertPtrToFlash(ARDUINOJSON_CONCAT2(name, _progmem)));
