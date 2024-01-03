// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Strings/JsonString.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/JsonVariantConst.hpp>

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

// A key-value pair.
// https://arduinojson.org/v7/api/jsonobject/begin_end/
class JsonPair {
 public:
  // INTERNAL USE ONLY
  JsonPair(detail::ObjectData::iterator iterator,
           detail::ResourceManager* resources)
      : iterator_(iterator), resources_(resources) {}

  // Returns the key.
  JsonString key() const {
    if (!iterator_.done())
      return JsonString(iterator_.key(), iterator_.ownsKey()
                                             ? JsonString::Copied
                                             : JsonString::Linked);
    else
      return JsonString();
  }

  // Returns the value.
  JsonVariant value() {
    return JsonVariant(iterator_.data(), resources_);
  }

 private:
  detail::ObjectData::iterator iterator_;
  detail::ResourceManager* resources_;
};

// A read-only key-value pair.
// https://arduinojson.org/v7/api/jsonobjectconst/begin_end/
class JsonPairConst {
 public:
  JsonPairConst(detail::ObjectData::iterator iterator,
                const detail::ResourceManager* resources)
      : iterator_(iterator), resources_(resources) {}

  // Returns the key.
  JsonString key() const {
    if (!iterator_.done())
      return JsonString(iterator_.key(), iterator_.ownsKey()
                                             ? JsonString::Copied
                                             : JsonString::Linked);
    else
      return JsonString();
  }

  // Returns the value.
  JsonVariantConst value() const {
    return JsonVariantConst(iterator_.data(), resources_);
  }

 private:
  detail::ObjectData::iterator iterator_;
  const detail::ResourceManager* resources_;
};

ARDUINOJSON_END_PUBLIC_NAMESPACE
