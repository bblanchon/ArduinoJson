// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/Adapters/RamString.hpp>
#include <ArduinoJson/Strings/String.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

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

template <>
struct StringAdapter<String> {
  typedef JsonStringAdapter AdaptedString;

  static AdaptedString adapt(const String& s) {
    return AdaptedString(s);
  }
};

}  // namespace ARDUINOJSON_NAMESPACE
