// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if T is a reference
template <typename T>
struct IsReference {
  static const bool value = false;
};

template <typename T>
struct IsReference<T&> {
  static const bool value = true;
};
}
}
