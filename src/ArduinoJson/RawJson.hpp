// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#pragma once

namespace ArduinoJson {

// A special type of data that can be used to insert pregenerated JSON portions.
class RawJson {
 public:
  explicit RawJson(const char* str) : _str(str) {}
  operator const char*() const {
    return _str;
  }

 private:
  const char* _str;
};
}
