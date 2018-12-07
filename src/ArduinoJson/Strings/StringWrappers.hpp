// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Memory/MemoryPool.hpp"
#include "../Polyfills/type_traits.hpp"

namespace ARDUINOJSON_NAMESPACE {
template <typename>
struct IsString : false_type {};

template <typename T>
struct IsString<const T> : IsString<T> {};

template <typename T>
struct IsString<T&> : IsString<T> {};
}  // namespace ARDUINOJSON_NAMESPACE

#include "ConstRamStringWrapper.hpp"
#include "RamStringWrapper.hpp"
#include "SizedRamStringWrapper.hpp"

#if ARDUINOJSON_ENABLE_STD_STRING
#include "StlStringWrapper.hpp"
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include "ArduinoStringWrapper.hpp"
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include "FlashStringWrapper.hpp"
#include "SizedFlashStringWrapper.hpp"
#endif
