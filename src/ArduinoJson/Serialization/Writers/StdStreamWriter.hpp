// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ostream>

namespace ARDUINOJSON_NAMESPACE {

template <typename TDestination>
class Writer<
    TDestination,
    typename enable_if<is_base_of<std::ostream, TDestination>::value>::type> {
 public:
  explicit Writer(std::ostream& os) : _os(&os) {}

  size_t write(uint8_t c) {
    _os->put(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    _os->write(reinterpret_cast<const char*>(s),
               static_cast<std::streamsize>(n));
    return n;
  }

 private:
  std::ostream* _os;
};
}  // namespace ARDUINOJSON_NAMESPACE
