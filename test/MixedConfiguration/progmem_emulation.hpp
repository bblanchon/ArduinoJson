// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <stdint.h>  // uint8_t
#include <string.h>  // strcmp, strlen...

class __FlashStringHelper;

inline const void* convertPtrToFlash(const void* s) {
  return reinterpret_cast<const char*>(s) + 42;
}

inline const void* convertFlashToPtr(const void* s) {
  return reinterpret_cast<const char*>(s) - 42;
}

#define F(X) reinterpret_cast<const __FlashStringHelper*>(convertPtrToFlash(X))
#define FC(X) reinterpret_cast<const char*>(convertPtrToFlash(X))

inline uint8_t pgm_read_byte(const void* p) {
  return *reinterpret_cast<const uint8_t*>(convertFlashToPtr(p));
}
