// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

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

ARDUINOJSON_END_PRIVATE_NAMESPACE
