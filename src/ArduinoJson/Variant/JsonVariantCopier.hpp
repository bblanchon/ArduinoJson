// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/JsonVariantVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

class JsonVariantCopier {
 public:
  typedef bool result_type;

  JsonVariantCopier(JsonVariant dst) : dst_(dst) {}

  template <typename T>
  bool visit(T src) {
    return dst_.set(src);
  }

 private:
  JsonVariant dst_;
};

inline bool copyVariant(JsonVariant dst, JsonVariantConst src) {
  if (dst.isUnbound())
    return false;
  JsonVariantCopier copier(dst);
  return accept(src, copier);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
