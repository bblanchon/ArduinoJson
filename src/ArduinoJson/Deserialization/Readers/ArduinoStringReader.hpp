// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of< ::String, TSource>::value>::type>
    : BoundedReader<const char*> {
  explicit Reader(const ::String& s)
      : BoundedReader<const char*>(s.c_str(), s.length()) {}
};

}  // namespace ARDUINOJSON_NAMESPACE
