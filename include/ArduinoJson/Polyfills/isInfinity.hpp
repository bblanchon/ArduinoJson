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

#ifdef __GLIBC__
template <>
inline bool isInfinity<double>(double x) {
  return isinfl(x);
}

template <>
inline bool isInfinity<float>(float x) {
  return isinff(x);
}
#endif
#endif
}
}
