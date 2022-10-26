// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/String.hpp>

namespace ARDUINOJSON_NAMESPACE {

class JsonStringAdapter : public SizedRamString {
 public:
  JsonStringAdapter(const String& s)
      : SizedRamString(s.c_str(), s.size()), _linked(s.isLinked()) {}

  StringStoragePolicy::LinkOrCopy storagePolicy() {
    StringStoragePolicy::LinkOrCopy policy = {_linked};
    return policy;
  }

 private:
  bool _linked;
};

inline JsonStringAdapter adaptString(const String& s) {
  return JsonStringAdapter(s);
}

template <>
struct IsString<String> : true_type {};

}  // namespace ARDUINOJSON_NAMESPACE
