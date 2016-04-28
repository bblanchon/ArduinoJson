// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"
#include "IsSame.hpp"
#include "IsSignedIntegral.hpp"
#include "IsUnsignedIntegral.hpp"

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if T is an integral type.
template <typename T>
struct IsIntegral {
  static const bool value = TypeTraits::IsSignedIntegral<T>::value ||
                            TypeTraits::IsUnsignedIntegral<T>::value ||
                            TypeTraits::IsSame<T, char>::value;
};
}
}
