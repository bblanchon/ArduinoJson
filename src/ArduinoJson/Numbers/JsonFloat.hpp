// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

#if ARDUINOJSON_USE_DOUBLE
typedef double JsonFloat;
#else
typedef float JsonFloat;
#endif

ARDUINOJSON_END_PUBLIC_NAMESPACE
