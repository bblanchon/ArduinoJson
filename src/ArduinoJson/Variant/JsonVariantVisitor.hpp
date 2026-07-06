// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2026, Benoit BLANCHON
// MIT License

#pragma once

#include <ArduinoJson/Array/JsonArray.hpp>
#include <ArduinoJson/Object/JsonObject.hpp>
#include <ArduinoJson/Variant/JsonVariant.hpp>
#include <ArduinoJson/Variant/VariantDataVisitor.hpp>

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TResult>
struct JsonVariantVisitor {
  using result_type = TResult;

  template <typename T>
  TResult visit(const T&) {
    return TResult();
  }
};

template <typename TVisitor>
class VisitorAdapter {
 public:
  using result_type = typename TVisitor::result_type;

  VisitorAdapter(TVisitor& visitor, ResourceManager* resources)
      : visitor_(&visitor), resources_(resources) {}

  result_type visitArray(VariantData* data) {
    return visitor_->visit(JsonArrayConst(data, resources_));
  }

  result_type visitObject(VariantData* data) {
    return visitor_->visit(JsonObjectConst(data, resources_));
  }

  template <typename T>
  result_type visit(const T& value) {
    return visitor_->visit(value);
  }

 private:
  TVisitor* visitor_;
  ResourceManager* resources_;
};

template <typename TVisitor>
typename TVisitor::result_type accept(JsonVariantConst variant,
                                      TVisitor& visit) {
  VisitorAdapter<TVisitor> adapter(
      visit, VariantAttorney::getResourceManager(variant));
  return VariantAttorney::getVariantImpl(variant).accept(adapter);
}

ARDUINOJSON_END_PRIVATE_NAMESPACE
