// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#ifdef ARDUINO  // assume this is an embedded platform

// store using float instead of double to reduce the memory usage (issue #134)
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 0
#endif

// store using a long because it usually match the size of a float.
#ifndef ARDUINOJSON_USE_LONG_LONG
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#ifndef ARDUINOJSON_USE_INT64
#define ARDUINOJSON_USE_INT64 0
#endif

// arduino has its own implementation of String to replace std::string
#ifndef ARDUINOJSON_USE_ARDUINO_STRING
#define ARDUINOJSON_USE_ARDUINO_STRING 1
#endif

// arduino doesn't support STL stream
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 0
#endif

#ifndef ARDUINOJSON_ENABLE_ALIGNMENT
#ifdef ARDUINO_ARCH_AVR
// alignment isn't needed for 8-bit AVR
#define ARDUINOJSON_ENABLE_ALIGNMENT 0
#else
// but must processor needs pointer to be align on word size
#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#endif
#endif

#else  // assume this is a computer

// on a computer we have plenty of memory so we can use doubles
#ifndef ARDUINOJSON_USE_DOUBLE
#define ARDUINOJSON_USE_DOUBLE 1
#endif

// use long long when available
#ifndef ARDUINOJSON_USE_LONG_LONG
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1800)
#define ARDUINOJSON_USE_LONG_LONG 1
#else
#define ARDUINOJSON_USE_LONG_LONG 0
#endif
#endif

// use _int64 on old versions of Visual Studio
#ifndef ARDUINOJSON_USE_INT64
#if defined(_MSC_VER) && _MSC_VER <= 1700
#define ARDUINOJSON_USE_INT64 1
#else
#define ARDUINOJSON_USE_INT64 0
#endif
#endif

// on a computer, we can use std::string
#ifndef ARDUINOJSON_USE_ARDUINO_STRING
#define ARDUINOJSON_USE_ARDUINO_STRING 0
#endif

// on a computer, we can assume that the STL is there
#ifndef ARDUINOJSON_ENABLE_STD_STREAM
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#endif

#ifndef ARDUINOJSON_ENABLE_ALIGNMENT
// even if not required, most cpu's are faster with aligned pointers
#define ARDUINOJSON_ENABLE_ALIGNMENT 1
#endif

#endif

#if ARDUINOJSON_USE_LONG_LONG && ARDUINOJSON_USE_INT64
#error ARDUINOJSON_USE_LONG_LONG and ARDUINOJSON_USE_INT64 cannot be set together
#endif
