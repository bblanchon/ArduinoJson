// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

#if ARDUINOJSON_USE_DOUBLE
using JsonFloat = double;
#else
using JsonFloat = float;
#endif

ARDUINOJSON_END_PUBLIC_NAMESPACE
