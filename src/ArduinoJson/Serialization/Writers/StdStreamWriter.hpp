// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ostream>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TDestination>
class Writer<TDestination,
             enable_if_t<is_base_of<std::ostream, TDestination>::value>> {
 public:
  explicit Writer(std::ostream& os) : os_(&os) {}

  size_t write(uint8_t c) {
    os_->put(static_cast<char>(c));
    return 1;
  }

  size_t write(const uint8_t* s, size_t n) {
    os_->write(reinterpret_cast<const char*>(s),
               static_cast<std::streamsize>(n));
    return n;
  }

 private:
  std::ostream* os_;
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
