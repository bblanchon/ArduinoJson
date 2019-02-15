// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
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

#include "ConstRamStringAdapter.hpp"
#include "RamStringAdapter.hpp"
#include "SizedRamStringAdapter.hpp"

#if ARDUINOJSON_ENABLE_STD_STRING
#include "StlStringAdapter.hpp"
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include "ArduinoStringAdapter.hpp"
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include "FlashStringAdapter.hpp"
#include "SizedFlashStringAdapter.hpp"
#endif
