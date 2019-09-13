// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Serialization/WriterSelector.hpp>

namespace ARDUINOJSON_NAMESPACE {

class PrintWriter {
 public:
  explicit PrintWriter(Print& print) : _print(print) {}

  size_t write(uint8_t c) {
    return _print.write(c);
  }

  size_t write(const uint8_t* s, size_t n) {
    return _print.write(s, n);
  }

 private:
  // cannot be assigned
  PrintWriter& operator=(const PrintWriter&);

  Print& _print;
};

template <typename TDestination>
struct WriterSelector<
    TDestination,
    typename enable_if<is_base_of<Print, TDestination>::value>::type> {
  typedef PrintWriter writer_type;
};
}  // namespace ARDUINOJSON_NAMESPACE
