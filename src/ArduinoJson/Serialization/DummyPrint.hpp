// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

class DummyPrint {
 public:
  size_t print(char) {
    return 1;
  }

  size_t print(const char* s) {
    return strlen(s);
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
