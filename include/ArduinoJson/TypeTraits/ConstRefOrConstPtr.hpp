// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that return the type T without the const modifier
template <typename T>
struct ConstRefOrConstPtr {
  typedef const T& type;
};
template <typename T>
struct ConstRefOrConstPtr<T*> {
  typedef const T* type;
};
template <typename T>
struct ConstRefOrConstPtr<T[]> {
  typedef const T* type;
};
template <typename T, size_t N>
struct ConstRefOrConstPtr<T[N]> {
  typedef const T* type;
};
}
}
