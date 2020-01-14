// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#if defined(_MSC_VER)
#define ARDUINOJSON_HAS_INT64 1
#else
#define ARDUINOJSON_HAS_INT64 0
#endif

#if __cplusplus >= 201103L
#define ARDUINOJSON_HAS_LONG_LONG 1
#define ARDUINOJSON_HAS_NULLPTR 1
#else
#define ARDUINOJSON_HAS_LONG_LONG 0
#define ARDUINOJSON_HAS_NULLPTR 0
#endif

// Small or big machine?
#ifndef ARDUINOJSON_EMBEDDED_MODE
#if defined(ARDUINO)                /* Arduino*/                 \
    || defined(__IAR_SYSTEMS_ICC__) /* IAR Embedded Workbench */ \
    || defined(__XC)                /* MPLAB XC compiler */      \
    || defined(__ARMCC_VERSION)     /* Keil ARM Compiler */      \
    || defined(__AVR)               /* Atmel AVR8/GNU C Compiler */
#define ARDUINOJSON_EMBEDDED_MODE 1
#else
#define ARDUINOJSON_EMBEDDED_MODE 0
#endif
#endif

// Auto enable std::stream if the right headers are here and no conflicting
// macro is defined
#if !defined(ARDUINOJSON_ENABLE_STD_STREAM) && defined(__has_include)
#if __has_include(<istream>) && \
    __has_include(<ostream>) && \
    !defined(min) && \
    !defined(max)
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#else
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#endif
#endif

// Auto enable std::string if the right header is here and no conflicting
// macro is defined
#if !defined(ARDUINOJSON_ENABLE_STD_STRING) && defined(__has_include)
#if __has_include(<string>) && !defined(min) && !defined(max)
#define ARDUINOJSON_ENABLE_STD_STRING 1
#else
#define ARDUINOJSON_ENABLE_STD_STRING 0
#endif
#endif

#if ARDUINOJSON_EMBEDDED_MODE

// Store floats by default to reduce the memory usage (issue #134)
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 0
#endif

// Store longs by default, because they usually match the size of a float.
#ifndef ARDUINOJSON_USE_LONG_LONG
#define ARDUINOJSON_USE_LONG_LONG 0
#endif

// Embedded systems usually don't have std::string
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 0
#endif

// Embedded systems usually don't have std::stream
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#endif

// Limit nesting as the stack is likely to be small
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 10
#endif

#else  // ARDUINOJSON_EMBEDDED_MODE

// On a computer we have plenty of memory so we can use doubles
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 1
#endif

// Use long long when available
#ifndef ARDUINOJSON_USE_LONG_LONG
#if ARDUINOJSON_HAS_LONG_LONG || ARDUINOJSON_HAS_INT64
#define ARDUINOJSON_USE_LONG_LONG 1
#else
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#endif

// On a computer, we can use std::string
#ifndef ARDUINOJSON_ENABLE_STD_STRING
#define ARDUINOJSON_ENABLE_STD_STRING 1
#endif

// On a computer, we can assume std::stream
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#endif

// On a computer, the stack is large so we can increase nesting limit
#ifndef ARDUINOJSON_DEFAULT_NESTING_LIMIT
#define ARDUINOJSON_DEFAULT_NESTING_LIMIT 50
#endif

#endif  // ARDUINOJSON_EMBEDDED_MODE

#ifdef ARDUINO

#include <Arduino.h>

// Enable support for Arduino's String class
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#endif

// Enable support for Arduino's Stream class
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#endif

// Enable support for Arduino's Print class
#ifndef ARDUINOJSON_ENABLE_ARDUINO_PRINT
#define ARDUINOJSON_ENABLE_ARDUINO_PRINT 1
#endif

#else  // ARDUINO

// Disable support for Arduino's String class
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STRING
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 0
#endif

// Disable support for Arduino's Stream class
#ifndef ARDUINOJSON_ENABLE_ARDUINO_STREAM
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 0
#endif

// Disable support for Arduino's Print class
#ifndef ARDUINOJSON_ENABLE_ARDUINO_PRINT
#define ARDUINOJSON_ENABLE_ARDUINO_PRINT 0
#endif

#endif  // ARDUINO

#ifndef ARDUINOJSON_ENABLE_PROGMEM
#ifdef PROGMEM
#define ARDUINOJSON_ENABLE_PROGMEM 1
#else
#define ARDUINOJSON_ENABLE_PROGMEM 0
#endif
#endif

// Convert unicode escape sequence (\u0123) to UTF-8
#ifndef ARDUINOJSON_DECODE_UNICODE
#define ARDUINOJSON_DECODE_UNICODE 0
#endif

// Ignore comments in input
#ifndef ARDUINOJSON_ENABLE_COMMENTS
#define ARDUINOJSON_ENABLE_COMMENTS 0
#endif

// Support NaN in JSON
#ifndef ARDUINOJSON_ENABLE_NAN
#define ARDUINOJSON_ENABLE_NAN 0
#endif

// Support Infinity in JSON
#ifndef ARDUINOJSON_ENABLE_INFINITY
#define ARDUINOJSON_ENABLE_INFINITY 0
#endif

// Control the exponentiation threshold for big numbers
// CAUTION: cannot be more that 1e9 !!!!
#ifndef ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD
#define ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD 1e7
#endif

// Control the exponentiation threshold for small numbers
#ifndef ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD
#define ARDUINOJSON_NEGATIVE_EXPONENTIATION_THRESHOLD 1e-5
#endif

#ifndef ARDUINOJSON_LITTLE_ENDIAN
#if defined(_MSC_VER) ||                                                      \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
    defined(__LITTLE_ENDIAN__) || defined(__i386) || defined(__x86_64)
#define ARDUINOJSON_LITTLE_ENDIAN 1
#else
#define ARDUINOJSON_LITTLE_ENDIAN 0
#endif
#endif

#ifndef ARDUINOJSON_TAB
#define ARDUINOJSON_TAB "  "
#endif

#ifndef ARDUINOJSON_STRING_BUFFER_SIZE
#define ARDUINOJSON_STRING_BUFFER_SIZE 32
#endif
