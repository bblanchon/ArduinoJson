// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Polyfills/preprocessor.hpp>
#include <ArduinoJson/version.hpp>

#ifndef ARDUINOJSON_VERSION_NAMESPACE

#  define ARDUINOJSON_VERSION_NAMESPACE                                       \
    ARDUINOJSON_CONCAT3(                                                      \
        ARDUINOJSON_CONCAT4(V, ARDUINOJSON_VERSION_MAJOR,                     \
                            ARDUINOJSON_VERSION_MINOR,                        \
                            ARDUINOJSON_VERSION_REVISION),                    \
        ARDUINOJSON_BIN2ALPHA(                                                \
            ARDUINOJSON_ENABLE_PROGMEM, ARDUINOJSON_USE_LONG_LONG,            \
            ARDUINOJSON_USE_DOUBLE, ARDUINOJSON_ENABLE_STRING_DEDUPLICATION), \
        ARDUINOJSON_BIN2ALPHA(                                                \
            ARDUINOJSON_ENABLE_NAN, ARDUINOJSON_ENABLE_INFINITY,              \
            ARDUINOJSON_ENABLE_COMMENTS, ARDUINOJSON_DECODE_UNICODE))

#endif

#define ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE \
  namespace ArduinoJson {                  \
  inline namespace ARDUINOJSON_VERSION_NAMESPACE {

#define ARDUINOJSON_END_PUBLIC_NAMESPACE \
  }                                      \
  }

#define ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE        \
  namespace ArduinoJson {                          \
  inline namespace ARDUINOJSON_VERSION_NAMESPACE { \
  namespace detail {

#define ARDUINOJSON_END_PRIVATE_NAMESPACE \
  }                                       \
  }                                       \
  }
