// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#ifdef _MSC_VER  // Visual Studio

#define FORCE_INLINE  // __forceinline causes C4714 when returning std::string
#define NO_INLINE __declspec(noinline)
#define DEPRECATED(msg) __declspec(deprecated(msg))

#elif defined(__GNUC__)  // GCC or Clang

#define FORCE_INLINE __attribute__((always_inline))
#define NO_INLINE __attribute__((noinline))
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define DEPRECATED(msg) __attribute__((deprecated))
#endif

#else  // Other compilers

#define FORCE_INLINE
#define NO_INLINE
#define DEPRECATED(msg)

#endif

#if __cplusplus >= 201103L
#define NOEXCEPT noexcept
#else
#define NOEXCEPT throw()
#endif

#if defined(__has_attribute)
#if __has_attribute(no_sanitize)
#define ARDUINOJSON_NO_SANITIZE(check) __attribute__((no_sanitize(check)))
#else
#define ARDUINOJSON_NO_SANITIZE(check)
#endif
#else
#define ARDUINOJSON_NO_SANITIZE(check)
#endif
