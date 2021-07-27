// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>
#include <ArduinoJson/Strings/StringAdapter.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TChar>
class StringAdapter<TChar*, false,
                    typename enable_if<sizeof(TChar) == 1 &&
                                       !is_same<TChar, void>::value>::type>
    : public StringAdapter<const char*> {
 public:
  StringAdapter(const TChar* str)
      : StringAdapter<const char*>(reinterpret_cast<const char*>(str)) {}

  void copyTo(char* p, size_t n) const {
    memcpy(p, _str, n);
  }

  typedef ARDUINOJSON_NAMESPACE::storage_policies::store_by_copy storage_policy;
};

}  // namespace ARDUINOJSON_NAMESPACE
