// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// The default writer is a simple wrapper for Writers that are not copiable
template <typename TDestination, typename Enable = void>
class Writer {
 public:
  explicit Writer(TDestination& dest) : _dest(&dest) {}

  size_t write(uint8_t c) {
    return _dest->write(c);
  }

  size_t write(const uint8_t* s, size_t n) {
    return _dest->write(s, n);
  }

 private:
  TDestination* _dest;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE

#include <ArduinoJson/Serialization/Writers/StaticStringWriter.hpp>

#if ARDUINOJSON_ENABLE_STD_STRING
#  include <ArduinoJson/Serialization/Writers/StdStringWriter.hpp>
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_STRING
#  include <ArduinoJson/Serialization/Writers/ArduinoStringWriter.hpp>
#endif

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ArduinoJson/Serialization/Writers/StdStreamWriter.hpp>
#endif

#if ARDUINOJSON_ENABLE_ARDUINO_PRINT
#  include <ArduinoJson/Serialization/Writers/PrintWriter.hpp>
#endif
