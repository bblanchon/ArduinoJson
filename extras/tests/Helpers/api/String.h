// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <string>

// Reproduces Arduino's String class
class String {
 public:
  String() = default;
  String(const char* s) {
    if (s)
      str_.assign(s);
  }

  void limitCapacityTo(size_t maxCapacity) {
    maxCapacity_ = maxCapacity;
  }

  unsigned char concat(const char* s) {
    return concat(s, strlen(s));
  }

  size_t length() const {
    return str_.size();
  }

  const char* c_str() const {
    return str_.c_str();
  }

  bool operator==(const char* s) const {
    return str_ == s;
  }

  String& operator=(const char* s) {
    if (s)
      str_.assign(s);
    else
      str_.clear();
    return *this;
  }

  char operator[](unsigned int index) const {
    if (index >= str_.size())
      return 0;
    return str_[index];
  }

  friend std::ostream& operator<<(std::ostream& lhs, const ::String& rhs) {
    lhs << rhs.str_;
    return lhs;
  }

 protected:
  // This function is protected in most Arduino cores
  unsigned char concat(const char* s, size_t n) {
    if (str_.size() + n > maxCapacity_)
      return 0;
    str_.append(s, n);
    return 1;
  }

 private:
  std::string str_;
  size_t maxCapacity_ = 1024;
};

class StringSumHelper : public ::String {};

inline bool operator==(const std::string& lhs, const ::String& rhs) {
  return lhs == rhs.c_str();
}
