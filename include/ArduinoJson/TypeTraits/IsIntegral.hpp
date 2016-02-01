// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "IsSame.hpp"

#include <stdint.h>

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if T is an integral type.
template <typename T>
struct IsIntegral {
  static const bool value = TypeTraits::IsSame<T, signed char>::value ||
                            TypeTraits::IsSame<T, unsigned char>::value ||
                            TypeTraits::IsSame<T, signed short>::value ||
                            TypeTraits::IsSame<T, unsigned short>::value ||
                            TypeTraits::IsSame<T, signed int>::value ||
                            TypeTraits::IsSame<T, unsigned int>::value ||
                            TypeTraits::IsSame<T, signed long>::value ||
                            TypeTraits::IsSame<T, unsigned long>::value ||
#ifndef ARDUINO
                            // on a computer add support for 64 bit
                            TypeTraits::IsSame<T, int64_t>::value ||
                            TypeTraits::IsSame<T, uint64_t>::value ||
#endif
                            TypeTraits::IsSame<T, char>::value;
};
}
}
