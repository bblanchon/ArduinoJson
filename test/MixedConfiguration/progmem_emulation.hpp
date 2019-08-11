// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <stdint.h>  // uint8_t
#include <string.h>  // strcmp, strlen...

class __FlashStringHelper;

typedef char prog_char;
typedef void prog_void;

inline const void* convertPtrToFlash(const void* s) {
  return reinterpret_cast<const char*>(s) + 42;
}

inline const void* convertFlashToPtr(const void* s) {
  return reinterpret_cast<const char*>(s) - 42;
}

#define F(X) reinterpret_cast<const __FlashStringHelper*>(convertPtrToFlash(X))

inline uint8_t pgm_read_byte_near(const void* p) {
  return *reinterpret_cast<const uint8_t*>(convertFlashToPtr(p));
}

inline void* memcpy_P(void* a, const prog_void* b, size_t n) {
  return memcpy(a, convertFlashToPtr(b), n);
}

inline int strcmp_P(const char* a, const prog_char* b) {
  return strcmp(a, reinterpret_cast<const char*>(convertFlashToPtr(b)));
}

inline int strncmp_P(const char* a, const prog_char* b, size_t n) {
  return strncmp(a, reinterpret_cast<const char*>(convertFlashToPtr(b)), n);
}

inline size_t strlen_P(const prog_char* s) {
  return strlen(reinterpret_cast<const char*>(convertFlashToPtr(s)));
}
