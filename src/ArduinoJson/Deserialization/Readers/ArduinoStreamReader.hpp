// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <Arduino.h>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of<Stream, TSource>::value>::type> {
 public:
  explicit Reader(Stream& stream) : _stream(&stream) {}

  int read() {
    // don't use _stream.read() as it ignores the timeout
    char c;
    return _stream->readBytes(&c, 1) ? static_cast<unsigned char>(c) : -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    return _stream->readBytes(buffer, length);
  }

 private:
  Stream* _stream;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
