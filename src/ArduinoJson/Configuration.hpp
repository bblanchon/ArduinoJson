// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#if __cplusplus >= 201103L
#  define ARDUINOJSON_HAS_LONG_LONG 1
#  define ARDUINOJSON_HAS_RVALUE_REFERENCES 1
#else
#  define ARDUINOJSON_HAS_LONG_LONG 0
#  define ARDUINOJSON_HAS_RVALUE_REFERENCES 0
#endif

#ifndef ARDUINOJSON_HAS_NULLPTR
#  if __cplusplus >= 201103L
#    define ARDUINOJSON_HAS_NULLPTR 1
#  else
#    define ARDUINOJSON_HAS_NULLPTR 0
#  endif
#endif

#if defined(_MSC_VER) && !ARDUINOJSON_HAS_LONG_LONG
#  define ARDUINOJSON_HAS_INT64 1
#else
#  define ARDUINOJSON_HAS_INT64 0
#endif

// Support std::istream and std::ostream
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#  ifdef __has_include
#    if __has_include(<istream>) && \
    __has_include(<ostream>) && \
    !defined(min) && \
    !defined(max)
#      define ARDUINOJSON_ENABLE_STD_STREAM 1
#    else
#      define ARDUINOJSON_ENABLE_STD_STREAM 0
#    endif
#  else
#    ifdef ARDUINO
#      define ARDUINOJSON_ENABLE_STD_STREAM 0
#    else
#      define ARDUINOJSON_ENABLE_STD_STREAM 1
#    endif
#  endif
#endif

// Support std::string
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#  ifdef __has_include
#    if __has_include(<string>) && !defined(min) && !defined(max)
#      define ARDUINOJSON_ENABLE_STD_STRING 1
#    else
#      define ARDUINOJSON_ENABLE_STD_STRING 0
#    endif
#  else
#    ifdef ARDUINO
#      define ARDUINOJSON_ENABLE_STD_STRING 0
#    else
#      define ARDUINOJSON_ENABLE_STD_STRING 1
#    endif
#  endif
#endif

// Support for std::string_view
#ifndef ARDUINOJSON_ENABLE_STRING_VIEW
#  ifdef __has_include
#    if __has_include(<string_view>) && __cplusplus >= 201703L
#      define ARDUINOJSON_ENABLE_STRING_VIEW 1
#    else
#      define ARDUINOJSON_ENABLE_STRING_VIEW 0
#    endif
#  else
#    define ARDUINOJSON_ENABLE_STRING_VIEW 0
#  endif
#endif

// Store floating-point values with float (0) or double (1)
#ifndef ARDUINOJSON_USE_DOUBLE
#  define ARDUINOJSON_USE_DOUBLE 1
#endif

// Store integral values with long (0) or long long (1)
#ifndef ARDUINOJSON_USE_LONG_LONG
#  if ARDUINOJSON_HAS_LONG_LONG && defined(__SIZEOF_POINTER__) && \
          __SIZEOF_POINTER__ >= 4 ||                              \
      defined(_MSC_VER)
#    define ARDUINOJSON_USE_LONG_LONG 1
#  endif
#endif
#ifndef ARDUINOJSON_USE_LONG_LONG
#  define ARDUINOJSON_USE_LONG_LONG 0
#endif

// Limit nesting as the stack is likely to be small
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#  define ARDUINOJSON_DEFAULT_NESTING_LIMIT 10
#endif

// Number of bits to store the pointer to next node
// (saves RAM but limits the number of values in a document)
#ifndef ARDUINOJSON_SLOT_OFFSET_SIZE
#  if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ <= 2
// Address space == 16-bit => max 127 values
#    define ARDUINOJSON_SLOT_OFFSET_SIZE 1
#  elif defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ >= 8 || \
      defined(_WIN64) && _WIN64
// Address space == 64-bit => max 2147483647 values
#    define ARDUINOJSON_SLOT_OFFSET_SIZE 4
#  else
// Address space == 32-bit => max 32767 values
#    define ARDUINOJSON_SLOT_OFFSET_SIZE 2
#  endif
#endif

#ifdef ARDUINO

// Enable support for Arduino's String class
#  ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#    define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#    include <Arduino.h>
#  endif

// Enable support for Arduino's Stream class
#  ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#    define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#    include <Arduino.h>
#  endif

// Enable support for Arduino's Print class
#  ifndef ARDUINOJSON_ENABLE_ARDUINO_PRINT
#    define ARDUINOJSON_ENABLE_ARDUINO_PRINT 1
#    include <Arduino.h>
#  endif

#else  // ARDUINO

// Disable support for Arduino's String class
#  ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#    define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
#  endif

// Disable support for Arduino's Stream class
#  ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#    define ARDUINOJSON_ENABLE_ARDUINO_STREAM 0
#  endif

// Disable support for Arduino's Print class
#  ifndef ARDUINOJSON_ENABLE_ARDUINO_PRINT
#    define ARDUINOJSON_ENABLE_ARDUINO_PRINT 0
#  endif

#endif  // ARDUINO

#ifndef ARDUINOJSON_ENABLE_PROGMEM
#  if defined(PROGMEM) && defined(pgm_read_byte) && defined(pgm_read_dword) && \
      defined(pgm_read_ptr) && defined(pgm_read_float)
#    ifdef ARDUINO
#      include <Arduino.h>
#    endif
#    define ARDUINOJSON_ENABLE_PROGMEM 1
#  else
#    define ARDUINOJSON_ENABLE_PROGMEM 0
#  endif
#endif

// Convert unicode escape sequence (\u0123) to UTF-8
#ifndef ARDUINOJSON_DECODE_UNICODE
#  define ARDUINOJSON_DECODE_UNICODE 1
#endif

// Ignore comments in input
#ifndef ARDUINOJSON_ENABLE_COMMENTS
#  define ARDUINOJSON_ENABLE_COMMENTS 0
#endif

// Support NaN in JSON
#ifndef ARDUINOJSON_ENABLE_NAN
#  define ARDUINOJSON_ENABLE_NAN 0
#endif

// Support Infinity in JSON
#ifndef ARDUINOJSON_ENABLE_INFINITY
#  define ARDUINOJSON_ENABLE_INFINITY 0
#endif

// Control the exponentiation threshold for big numbers
// CAUTION: cannot be more that 1e9 !!!!
#ifndef ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD
#  define ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD 1e7
#endif

// Control the exponentiation threshold for small numbers
#ifndef ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD
#  define ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD 1e-5
#endif

#ifndef ARDUINOJSON_LITTLE_ENDIAN
#  if defined(_MSC_VER) ||                           \
      (defined(__BYTE_ORDER__) &&                    \
       __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
      defined(__LITTLE_ENDIAN__) || defined(__i386) || defined(__x86_64)
#    define ARDUINOJSON_LITTLE_ENDIAN 1
#  else
#    define ARDUINOJSON_LITTLE_ENDIAN 0
#  endif
#endif

#ifndef ARDUINOJSON_ENABLE_ALIGNMENT
#  if defined(__AVR)
#    define ARDUINOJSON_ENABLE_ALIGNMENT 0
#  else
#    define ARDUINOJSON_ENABLE_ALIGNMENT 1
#  endif
#endif

#ifndef ARDUINOJSON_TAB
#  define ARDUINOJSON_TAB "  "
#endif

#ifndef ARDUINOJSON_ENABLE_STRING_DEDUPLICATION
#  define ARDUINOJSON_ENABLE_STRING_DEDUPLICATION 1
#endif

#ifndef ARDUINOJSON_STRING_BUFFER_SIZE
#  define ARDUINOJSON_STRING_BUFFER_SIZE 32
#endif

#ifndef ARDUINOJSON_DEBUG
#  ifdef __PLATFORMIO_BUILD_DEBUG__
#    define ARDUINOJSON_DEBUG 1
#  else
#    define ARDUINOJSON_DEBUG 0
#  endif
#endif

#if ARDUINOJSON_HAS_NULLPTR && defined(nullptr)
#  error nullptr is defined as a macro. Remove the faulty #define or #undef nullptr
// See https://github.com/bblanchon/ArduinoJson/issues/1355
#endif
