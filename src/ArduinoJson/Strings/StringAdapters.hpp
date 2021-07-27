// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/ConstRamStringAdapter.hpp>
#include <ArduinoJson/Strings/Adapters/JsonStringAdapter.hpp>
#include <ArduinoJson/Strings/Adapters/RamStringAdapter.hpp>
#include <ArduinoJson/Strings/Adapters/SizedRamStringAdapter.hpp>

#if ARDUINOJSON_ENABLE_STD_STRING
#  include <ArduinoJson/Strings/Adapters/StdStringAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_STRING_VIEW
#  include <ArduinoJson/Strings/Adapters/StringViewAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#  include <ArduinoJson/Strings/Adapters/ArduinoStringAdapter.hpp>
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
#  include <ArduinoJson/Strings/Adapters/FlashStringAdapter.hpp>
#  include <ArduinoJson/Strings/Adapters/SizedFlashStringAdapter.hpp>
#endif
