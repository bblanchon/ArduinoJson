// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Strings/StoredString.hpp>
#include <ArduinoJson/Strings/String.hpp>

namespace ARDUINOJSON_NAMESPACE {

struct LinkStringStoragePolicy {
  template <typename TAdaptedString, typename TCallback>
  bool store(TAdaptedString str, MemoryPool *, TCallback callback) {
    LinkedString storedString(str.data(), str.size());
    callback(storedString);
    return !str.isNull();
  }
};

struct CopyStringStoragePolicy {
  typedef CopiedString TResult;

  template <typename TAdaptedString, typename TCallback>
  bool store(TAdaptedString str, MemoryPool *pool, TCallback callback);
};

class LinkOrCopyStringStoragePolicy : LinkStringStoragePolicy,
                                      CopyStringStoragePolicy {
 public:
  LinkOrCopyStringStoragePolicy(bool link) : _link(link) {}

  template <typename TAdaptedString, typename TCallback>
  bool store(TAdaptedString str, MemoryPool *pool, TCallback callback) {
    if (_link)
      return LinkStringStoragePolicy::store(str, pool, callback);
    else
      return CopyStringStoragePolicy::store(str, pool, callback);
  }

 private:
  bool _link;
};

template <typename T>
inline CopyStringStoragePolicy getStringStoragePolicy(const T &) {
  return CopyStringStoragePolicy();
}

inline LinkStringStoragePolicy getStringStoragePolicy(const char *) {
  return LinkStringStoragePolicy();
}

inline LinkOrCopyStringStoragePolicy getStringStoragePolicy(const String &s) {
  return LinkOrCopyStringStoragePolicy(s.isStatic());
}

}  // namespace ARDUINOJSON_NAMESPACE
