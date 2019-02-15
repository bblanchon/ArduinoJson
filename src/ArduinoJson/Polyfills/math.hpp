// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {
template <typename T>
bool isNaN(T x) {
  return x != x;
}

template <typename T>
bool isInfinity(T x) {
  return x != 0.0 && x * 2 == x;
}
}  // namespace Internals
}  // namespace ArduinoJson
