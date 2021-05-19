// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <sstream>

class CustomReader {
  std::stringstream _stream;

 public:
  CustomReader(const char* input) : _stream(input) {}

  int read() {
    return _stream.get();
  }

  size_t readBytes(char* buffer, size_t length) {
    _stream.read(buffer, static_cast<std::streamsize>(length));
    return static_cast<size_t>(_stream.gcount());
  }

 private:
  CustomReader(const CustomReader&);
};
