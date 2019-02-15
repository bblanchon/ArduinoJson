// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// A meta-function that return the type T if Condition is true.
template <bool Condition, typename T = void>
struct EnableIf {};

template <typename T>
struct EnableIf<true, T> {
  typedef T type;
};
}  // namespace Internals
}  // namespace ArduinoJson
