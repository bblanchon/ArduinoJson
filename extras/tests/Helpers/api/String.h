// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <string>

// Reproduces Arduino's String class
class String {
 public:
  String() {}
  explicit String(const char* s) : _str(s) {}

  String& operator+=(const char* rhs) {
    _str += rhs;
    return *this;
  }

  size_t length() const {
    return _str.size();
  }

  const char* c_str() const {
    return _str.c_str();
  }

  bool operator==(const char* s) const {
    return _str == s;
  }

  friend std::ostream& operator<<(std::ostream& lhs, const ::String& rhs) {
    lhs << rhs._str;
    return lhs;
  }

 private:
  std::string _str;
};

class StringSumHelper;

inline bool operator==(const std::string& lhs, const ::String& rhs) {
  return lhs == rhs.c_str();
}
