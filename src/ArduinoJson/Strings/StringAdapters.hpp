// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>

namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct IsString : false_type {};

template <typename T>
struct IsString<const T> : IsString<T> {};

template <typename T>
struct IsString<T&> : IsString<T> {};
}  // namespace ARDUINOJSON_NAMESPACE

#include <ArduinoJson/Strings/ConstRamStringAdapter.hpp>
#include <ArduinoJson/Strings/RamStringAdapter.hpp>
#include <ArduinoJson/Strings/SizedRamStringAdapter.hpp>

#if ARDUINOJSON_ENABLE_STD_STRING
#include <ArduinoJson/Strings/StlStringAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include <ArduinoJson/Strings/ArduinoStringAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include <ArduinoJson/Strings/FlashStringAdapter.hpp>
#include <ArduinoJson/Strings/SizedFlashStringAdapter.hpp>
#endif
