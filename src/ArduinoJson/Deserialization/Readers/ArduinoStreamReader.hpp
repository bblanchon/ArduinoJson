// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of<Stream, TSource>::value>::type> {
 public:
  explicit Reader(Stream& stream) : stream_(&stream) {}

  int read() {
    // don't use stream_->read() as it ignores the timeout
    char c;
    return stream_->readBytes(&c, 1) ? static_cast<unsigned char>(c) : -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    return stream_->readBytes(buffer, length);
  }

 private:
  Stream* stream_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
