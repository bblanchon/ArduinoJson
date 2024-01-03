// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Polyfills/preprocessor.hpp>
#include <ArduinoJson/version.hpp>

#ifndef ARDUINOJSON_VERSION_NAMESPACE

#  define ARDUINOJSON_VERSION_NAMESPACE                                    \
    ARDUINOJSON_CONCAT4(ARDUINOJSON_VERSION_MACRO,                         \
                        ARDUINOJSON_BIN2ALPHA(ARDUINOJSON_ENABLE_PROGMEM,  \
                                              ARDUINOJSON_USE_LONG_LONG,   \
                                              ARDUINOJSON_USE_DOUBLE, 1),  \
                        ARDUINOJSON_BIN2ALPHA(ARDUINOJSON_ENABLE_NAN,      \
                                              ARDUINOJSON_ENABLE_INFINITY, \
                                              ARDUINOJSON_ENABLE_COMMENTS, \
                                              ARDUINOJSON_DECODE_UNICODE), \
                        ARDUINOJSON_SLOT_ID_SIZE)

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
