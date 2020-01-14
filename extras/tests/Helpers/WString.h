// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <string>

// Reproduces Arduino's String class
class String {
 public:
  String& operator+=(char c) {
    _str += c;
    return *this;
  }
  String& operator+=(int);  // no used, just to add ambiguity

  unsigned char reserve(size_t capacity) {
    _str.reserve(capacity);
    return 1;
  }

  size_t length() const {
    return _str.size();
  }

  const char* c_str() const {
    return _str.c_str();
  }

 private:
  std::string _str;
};

class StringSumHelper;

bool operator==(const std::string& lhs, const ::String& rhs) {
  return lhs == rhs.c_str();
}
