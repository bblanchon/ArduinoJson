// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
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

inline const void* pgm_read_ptr(const void* p) {
  return *reinterpret_cast<const void* const*>(convertFlashToPtr(p));
}
