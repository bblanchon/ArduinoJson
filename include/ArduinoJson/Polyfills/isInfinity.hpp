// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

// If Visual Studo <= 2012
#if defined(_MSC_VER) && _MSC_VER <= 1700
#include <float.h>
#else
#include <math.h>
#endif

// GCC warning: "conversion to 'float' from 'double' may alter its value"
#ifdef __GNUC__
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#else
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#endif

namespace ArduinoJson {
namespace Polyfills {

// If Visual Studo <= 2012
#if defined(_MSC_VER) && _MSC_VER <= 1700
template <typename T>
bool isInfinity(T x) {
  return !_finite(x);
}
#else
template <typename T>
bool isInfinity(T x) {
  return isinf(x);
}

#if defined(_GLIBCXX_HAVE_ISINFL) && _GLIBCXX_HAVE_ISINFL
template <>
inline bool isInfinity<double>(double x) {
  return isinfl(x);
}
#endif

#if defined(_GLIBCXX_HAVE_ISINFF) && _GLIBCXX_HAVE_ISINFF
template <>
inline bool isInfinity<float>(float x) {
  return isinff(x);
}
#endif
#endif
}
}

#if defined(__GNUC__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif
