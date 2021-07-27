// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamStringAdapter.hpp>
#include <ArduinoJson/Strings/String.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <>
class StringAdapter<String> : public StringAdapter<char*> {
 public:
  StringAdapter(const String& str)
      : StringAdapter<char*>(str.c_str()), _isStatic(str.isStatic()) {}

  bool isStatic() const {
    return _isStatic;
  }

  typedef storage_policies::decide_at_runtime storage_policy;

 private:
  bool _isStatic;
};
}  // namespace ARDUINOJSON_NAMESPACE
