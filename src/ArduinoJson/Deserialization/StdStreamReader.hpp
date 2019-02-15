// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <istream>

namespace ARDUINOJSON_NAMESPACE {

class StdStreamReader {
  std::istream& _stream;
  char _current;

 public:
  explicit StdStreamReader(std::istream& stream)
      : _stream(stream), _current(0) {}

  bool ended() const {
    return _stream.eof();
  }

  char read() {
    return static_cast<char>(_stream.get());
  }

 private:
  StdStreamReader& operator=(const StdStreamReader&);  // Visual Studio C4512
};

inline StdStreamReader makeReader(std::istream& input) {
  return StdStreamReader(input);
}
}  // namespace ARDUINOJSON_NAMESPACE

#endif
