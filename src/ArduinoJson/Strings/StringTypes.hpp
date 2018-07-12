// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Polyfills/type_traits.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename>
struct IsString : false_type {};

template <typename T>
struct IsString<const T> : IsString<T> {};

template <typename T>
struct IsString<T&> : IsString<T> {};
}  // namespace Internals
}  // namespace ArduinoJson

#include "FixedSizeRamString.hpp"
#include "ZeroTerminatedRamString.hpp"

#if ARDUINOJSON_ENABLE_STD_STRING
#include "StlString.hpp"
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include "ArduinoString.hpp"
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include "FixedSizeFlashString.hpp"
#include "ZeroTerminatedFlashString.hpp"
#endif
