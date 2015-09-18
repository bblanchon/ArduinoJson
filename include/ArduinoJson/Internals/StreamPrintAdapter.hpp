// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#ifdef ARDUINOJSON_ENABLE_STD_STREAM

#include "../Arduino/Print.hpp"

namespace ArduinoJson {
namespace Internals {

class StreamPrintAdapter : public Print {
 public:
  explicit StreamPrintAdapter(std::ostream& os) : _os(os) {}

  virtual size_t write(uint8_t c) {
    _os << static_cast<char>(c);
    return 1;
  }

 private:
  // cannot be assigned
  StreamPrintAdapter& operator=(const StreamPrintAdapter&);

  std::ostream& _os;
};
}
}

#endif // ARDUINOJSON_ENABLE_STD_STREAM
