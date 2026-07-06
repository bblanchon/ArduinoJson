// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>

#if ARDUINOJSON_DEBUG
#  include <assert.h>
#  define ARDUINOJSON_ASSERT(X) assert(X)
#else
#  define ARDUINOJSON_ASSERT(X) ((void)0)
#endif
