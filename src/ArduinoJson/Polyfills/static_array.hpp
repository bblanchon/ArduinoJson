// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>

#if ARDUINOJSON_ENABLE_PROGMEM

#  include <ArduinoJson/Polyfills/pgmspace_generic.hpp>

#  ifndef ARDUINOJSON_DEFINE_STATIC_ARRAY
#    define ARDUINOJSON_DEFINE_STATIC_ARRAY(type, name, value) \
      static type const name[] PROGMEM = value;
#  endif

#  ifndef ARDUINOJSON_READ_STATIC_ARRAY
#    define ARDUINOJSON_READ_STATIC_ARRAY(type, name, index) \
      pgm_read<type>(name + index)
#  endif

#else  // i.e. ARDUINOJSON_ENABLE_PROGMEM == 0

#  ifndef ARDUINOJSON_DEFINE_STATIC_ARRAY
#    define ARDUINOJSON_DEFINE_STATIC_ARRAY(type, name, value) \
      static type const name[] = value;
#  endif

#  ifndef ARDUINOJSON_READ_STATIC_ARRAY
#    define ARDUINOJSON_READ_STATIC_ARRAY(type, name, index) name[index]
#  endif

#endif
