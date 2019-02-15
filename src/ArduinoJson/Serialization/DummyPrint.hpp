// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// A dummy Print implementation used in JsonPrintable::measureLength()
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
