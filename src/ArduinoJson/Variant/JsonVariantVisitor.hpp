// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2023, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/VariantDataVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TResult>
struct JsonVariantVisitor {
  typedef TResult result_type;

  TResult visitArray(JsonArrayConst) {
    return TResult();
  }

  TResult visitBoolean(bool) {
    return TResult();
  }

  TResult visitFloat(JsonFloat) {
    return TResult();
  }

  TResult visitSignedInteger(JsonInteger) {
    return TResult();
  }

  TResult visitNull() {
    return TResult();
  }

  TResult visitObject(JsonObjectConst) {
    return TResult();
  }

  TResult visitUnsignedInteger(JsonUInt) {
    return TResult();
  }

  TResult visitRawString(const char*, size_t) {
    return TResult();
  }

  TResult visitString(JsonString) {
    return TResult();
  }
};

template <typename TVisitor>
class VisitorAdapter {
 public:
  using result_type = typename TVisitor::result_type;

  VisitorAdapter(TVisitor& visitor, const ResourceManager* resources)
      : visitor_(&visitor), resources_(resources) {}

  result_type visitArray(const ArrayData& array) {
    return visitor_->visitArray(JsonArrayConst(&array, resources_));
  }

  result_type visitObject(const ObjectData& object) {
    return visitor_->visitObject(JsonObjectConst(&object, resources_));
  }

  result_type visitFloat(JsonFloat value) {
    return visitor_->visitFloat(value);
  }

  result_type visitString(JsonString value) {
    return visitor_->visitString(value);
  }

  result_type visitRawString(const char* p, size_t n) {
    return visitor_->visitRawString(p, n);
  }

  result_type visitSignedInteger(JsonInteger value) {
    return visitor_->visitSignedInteger(value);
  }

  result_type visitUnsignedInteger(JsonUInt value) {
    return visitor_->visitUnsignedInteger(value);
  }

  result_type visitBoolean(bool value) {
    return visitor_->visitBoolean(value);
  }

  result_type visitNull() {
    return visitor_->visitNull();
  }

 private:
  TVisitor* visitor_;
  const ResourceManager* resources_;
};

template <typename TVisitor>
typename TVisitor::result_type accept(JsonVariantConst variant,
                                      TVisitor& visitor) {
  auto data = VariantAttorney::getData(variant);
  if (!data)
    return visitor.visitNull();
  auto resources = VariantAttorney::getResourceManager(variant);
  VisitorAdapter<TVisitor> adapter(visitor, resources);
  return data->accept(adapter);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
