// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#pragma once

#include "ConstRamStringAdapter.hpp"

namespace ARDUINOJSON_NAMESPACE {

class String {
 public:
  String() : _data(0), _isStatic(true) {}
  String(const char* data, bool isStaticData = true)
      : _data(data), _isStatic(isStaticData) {}

  const char* c_str() const {
    return _data;
  }

  bool isNull() const {
    return !_data;
  }

  bool isStatic() const {
    return _isStatic;
  }

  friend bool operator==(String lhs, String rhs) {
    if (lhs._data == rhs._data) return true;
    if (!lhs._data) return false;
    if (!rhs._data) return false;
    return strcmp(lhs._data, rhs._data) == 0;
  }

 private:
  const char* _data;
  bool _isStatic;
};

class StringAdapter : public RamStringAdapter {
 public:
  StringAdapter(const String& str)
      : RamStringAdapter(str.c_str()), _isStatic(str.isStatic()) {}

  bool isStatic() const {
    return _isStatic;
  }

  /*  const char* save(MemoryPool* pool) const {
      if (_isStatic) return c_str();
      return RamStringAdapter::save(pool);
    }*/

 private:
  bool _isStatic;
};

template <>
struct IsString<String> : true_type {};

inline StringAdapter adaptString(const String& str) {
  return StringAdapter(str);
}
}  // namespace ARDUINOJSON_NAMESPACE
