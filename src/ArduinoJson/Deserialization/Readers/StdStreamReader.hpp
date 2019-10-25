// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <istream>

namespace ARDUINOJSON_NAMESPACE {

template <typename TSource>
struct Reader<TSource, typename enable_if<
                           is_base_of<std::istream, TSource>::value>::type> {
 public:
  explicit Reader(std::istream& stream) : _stream(&stream) {}

  int read() {
    return _stream->get();
  }

  size_t readBytes(char* buffer, size_t length) {
    _stream->read(buffer, static_cast<std::streamsize>(length));
    return static_cast<size_t>(_stream->gcount());
  }

 private:
  std::istream* _stream;
};
}  // namespace ARDUINOJSON_NAMESPACE
