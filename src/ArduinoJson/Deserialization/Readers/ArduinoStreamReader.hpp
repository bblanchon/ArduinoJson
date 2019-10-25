// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <Stream.h>

namespace ARDUINOJSON_NAMESPACE {

template <typename TSource>
struct Reader<TSource,
              typename enable_if<is_base_of<Stream, TSource>::value>::type> {
  Stream& _stream;

 public:
  explicit Reader(Stream& stream) : _stream(stream) {}

  int read() {
    // don't use _stream.read() as it ignores the timeout
    char c;
    return _stream.readBytes(&c, 1) ? c : -1;
  }

  size_t readBytes(char* buffer, size_t length) {
    return _stream.readBytes(buffer, length);
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
