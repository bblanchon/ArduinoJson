// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#ifdef ARDUINOJSON_DEBUG
#include <assert.h>
#define ARDUINOJSON_ASSERT(X) assert(X)
#else
#define ARDUINOJSON_ASSERT(X) ((void)0)
#endif
