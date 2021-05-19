// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

template <typename TDestination>
class Writer<
    TDestination,
    typename enable_if<is_base_of< ::Print, TDestination>::value>::type> {
 public:
  explicit Writer(::Print& print) : _print(&print) {}

  size_t write(uint8_t c) {
    return _print->write(c);
  }

  size_t write(const uint8_t* s, size_t n) {
    return _print->write(s, n);
  }

 private:
  ::Print* _print;
};

}  // namespace ARDUINOJSON_NAMESPACE
