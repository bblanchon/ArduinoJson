// ArduinoJson - https://arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/safe_strcmp.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TStoragePolicy>
class StoredString {
 public:
  StoredString(const char* p) : _data(p) {}

  operator const char*() const {
    return _data;
  }

  const char* c_str() const {
    return _data;
  }

 private:
  const char* _data;
};

typedef StoredString<storage_policies::store_by_address> LinkedString;
typedef StoredString<storage_policies::store_by_copy> CopiedString;

}  // namespace ARDUINOJSON_NAMESPACE
