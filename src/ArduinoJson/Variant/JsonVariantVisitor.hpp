// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
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

  template <typename T>
  TResult visit(const T&) {
    return TResult();
  }
};

template <typename TVisitor>
class VisitorAdapter {
 public:
  using result_type = typename TVisitor::result_type;

  VisitorAdapter(TVisitor& visitor, const ResourceManager* resources)
      : visitor_(&visitor), resources_(resources) {}

  result_type visit(const ArrayData& value) {
    return visitor_->visit(JsonArrayConst(&value, resources_));
  }

  result_type visit(const ObjectData& value) {
    return visitor_->visit(JsonObjectConst(&value, resources_));
  }

  template <typename T>
  result_type visit(const T& value) {
    return visitor_->visit(value);
  }

 private:
  TVisitor* visitor_;
  const ResourceManager* resources_;
};

template <typename TVisitor>
typename TVisitor::result_type accept(JsonVariantConst variant,
                                      TVisitor& visit) {
  auto data = VariantAttorney::getData(variant);
  if (!data)
    return visit.visit(nullptr);
  auto resources = VariantAttorney::getResourceManager(variant);
  VisitorAdapter<TVisitor> adapter(visit, resources);
  return data->accept(adapter, resources);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
