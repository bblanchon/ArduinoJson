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
bool isNaN(T x) {
  return _isnan(x) != 0;
}
#else
template <typename T>
bool isNaN(T x) {
  return isnan(x);
}

#ifdef __GLIBC__
template <>
inline bool isNaN<double>(double x) {
  return isnanl(x);
}

template <>
inline bool isNaN<float>(float x) {
  return isnanf(x);
}
#endif

#endif
}
}
