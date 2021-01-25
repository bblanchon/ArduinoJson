// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TWriter>
class CountingDecorator {
 public:
  explicit CountingDecorator(TWriter& writer) : _writer(writer), _count(0) {}

  void write(uint8_t c) {
    _count += _writer.write(c);
  }

  void write(const uint8_t* s, size_t n) {
    _count += _writer.write(s, n);
  }

  size_t count() const {
    return _count;
  }

 private:
  TWriter _writer;
  size_t _count;
};

}  // namespace ARDUINOJSON_NAMESPACE
