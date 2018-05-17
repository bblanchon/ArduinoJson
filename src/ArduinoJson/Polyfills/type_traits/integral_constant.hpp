// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename T, T v>
struct integral_constant {
  static const T value = v;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

}  // namespace Internals
}  // namespace ArduinoJson
