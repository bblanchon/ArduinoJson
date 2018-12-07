// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ARDUINOJSON_NAMESPACE {

class String {
 public:
  String() : _data(0) {}
  String(const char* slot) : _data(slot) {}

  const char* c_str() const {
    return _data;
  }

  bool isNull() const {
    return !_data;
  }

  friend bool operator==(String lhs, String rhs) {
    if (lhs._data == rhs._data) return true;
    if (!lhs._data) return false;
    if (!rhs._data) return false;
    return strcmp(lhs._data, rhs._data) == 0;
  }

 private:
  const char* _data;
};
}  // namespace ARDUINOJSON_NAMESPACE
