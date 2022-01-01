// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Misc/SafeBoolIdiom.hpp>

#if ARDUINOJSON_ENABLE_STD_STREAM
#  include <ostream>
#endif

namespace ARDUINOJSON_NAMESPACE {

class String : public SafeBoolIdom<String> {
 public:
  String() : _data(0), _size(0), _isStatic(true) {}

  String(const char* data, bool isStaticData = true)
      : _data(data),
        _size(data ? ::strlen(data) : 0),
        _isStatic(isStaticData) {}

  String(const char* data, size_t sz, bool isStaticData = true)
      : _data(data), _size(sz), _isStatic(isStaticData) {}

  const char* c_str() const {
    return _data;
  }

  bool isNull() const {
    return !_data;
  }

  bool isStatic() const {
    return _isStatic;
  }

  size_t size() const {
    return _size;
  }

  // safe bool idiom
  operator bool_type() const {
    return _data ? safe_true() : safe_false();
  }

  friend bool operator==(String lhs, String rhs) {
    if (lhs._data == rhs._data)
      return true;
    if (!lhs._data)
      return false;
    if (!rhs._data)
      return false;
    return strcmp(lhs._data, rhs._data) == 0;
  }

  friend bool operator!=(String lhs, String rhs) {
    if (lhs._data == rhs._data)
      return false;
    if (!lhs._data)
      return true;
    if (!rhs._data)
      return true;
    return strcmp(lhs._data, rhs._data) != 0;
  }

#if ARDUINOJSON_ENABLE_STD_STREAM
  friend std::ostream& operator<<(std::ostream& lhs, const String& rhs) {
    lhs.write(rhs.c_str(), static_cast<std::streamsize>(rhs.size()));
    return lhs;
  }
#endif

 private:
  const char* _data;
  size_t _size;
  bool _isStatic;
};

}  // namespace ARDUINOJSON_NAMESPACE
