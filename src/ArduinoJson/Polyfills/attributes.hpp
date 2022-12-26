// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#ifdef _MSC_VER  // Visual Studio

#  define FORCE_INLINE  // __forceinline causes C4714 when returning std::string
#  define NO_INLINE __declspec(noinline)

#elif defined(__GNUC__)  // GCC or Clang

#  define FORCE_INLINE __attribute__((always_inline))
#  define NO_INLINE __attribute__((noinline))

#else  // Other compilers

#  define FORCE_INLINE
#  define NO_INLINE

#endif

#if __cplusplus >= 201103L
#  define NOEXCEPT noexcept
#else
#  define NOEXCEPT throw()
#endif

#if defined(__has_attribute)
#  if __has_attribute(no_sanitize)
#    define ARDUINOJSON_NO_SANITIZE(check) __attribute__((no_sanitize(check)))
#  else
#    define ARDUINOJSON_NO_SANITIZE(check)
#  endif
#else
#  define ARDUINOJSON_NO_SANITIZE(check)
#endif
