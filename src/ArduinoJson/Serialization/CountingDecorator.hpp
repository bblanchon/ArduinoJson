// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TWriter>
class CountingDecorator {
 public:
  explicit CountingDecorator(TWriter& writer) : writer_(writer), count_(0) {}

  void write(uint8_t c) {
    count_ += writer_.write(c);
  }

  void write(const uint8_t* s, size_t n) {
    count_ += writer_.write(s, n);
  }

  size_t count() const {
    return count_;
  }

 private:
  TWriter writer_;
  size_t count_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
