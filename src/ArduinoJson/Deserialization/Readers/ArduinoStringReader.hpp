// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of<::String, TSource>::value>::type>
    : BoundedReader<const char*> {
  explicit Reader(const ::String& s)
      : BoundedReader<const char*>(s.c_str(), s.length()) {}
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
