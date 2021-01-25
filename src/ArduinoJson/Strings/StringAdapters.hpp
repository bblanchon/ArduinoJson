// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/ConstRamStringAdapter.hpp>
#include <ArduinoJson/Strings/RamStringAdapter.hpp>
#include <ArduinoJson/Strings/SizedRamStringAdapter.hpp>

#if ARDUINOJSON_ENABLE_STD_STRING
#include <ArduinoJson/Strings/StdStringAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#include <ArduinoJson/Strings/ArduinoStringAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#include <ArduinoJson/Strings/FlashStringAdapter.hpp>
#include <ArduinoJson/Strings/SizedFlashStringAdapter.hpp>
#endif
