// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_USE_DOUBLE
typedef double JsonFloat;
#else
typedef float JsonFloat;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
