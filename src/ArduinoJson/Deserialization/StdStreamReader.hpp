// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <istream>

namespace ARDUINOJSON_NAMESPACE {

class StdStreamReader {
  std::istream& _stream;
  char _current;

 public:
  explicit StdStreamReader(std::istream& stream)
      : _stream(stream), _current(0) {}

  int read() {
    return _stream.get();
  }

  size_t readBytes(char* buffer, size_t length) {
    _stream.read(buffer, static_cast<std::streamsize>(length));
    return static_cast<size_t>(_stream.gcount());
  }

 private:
  StdStreamReader& operator=(const StdStreamReader&);  // Visual Studio C4512
};

inline StdStreamReader makeReader(std::istream& input) {
  return StdStreamReader(input);
}
}  // namespace ARDUINOJSON_NAMESPACE

#endif
