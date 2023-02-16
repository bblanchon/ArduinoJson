// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <string>

// Reproduces Arduino's String class
class String {
 public:
  String() : _maxCapacity(1024) {}
  explicit String(const char* s) : _str(s), _maxCapacity(1024) {}

  void limitCapacityTo(size_t maxCapacity) {
    _maxCapacity = maxCapacity;
  }

  unsigned char concat(const char* s) {
    return concat(s, strlen(s));
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

  String& operator=(const char* s) {
    _str.assign(s);
    return *this;
  }

  char operator[](unsigned int index) const {
    if (index >= _str.size())
      return 0;
    return _str[index];
  }

  friend std::ostream& operator<<(std::ostream& lhs, const ::String& rhs) {
    lhs << rhs._str;
    return lhs;
  }

 protected:
  // This function is protected in most Arduino cores
  unsigned char concat(const char* s, size_t n) {
    if (_str.size() + n > _maxCapacity)
      return 0;
    _str.append(s, n);
    return 1;
  }

 private:
  std::string _str;
  size_t _maxCapacity;
};

class StringSumHelper : public ::String {};

inline bool operator==(const std::string& lhs, const ::String& rhs) {
  return lhs == rhs.c_str();
}
