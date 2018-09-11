// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "JsonVariantContent.hpp"
#include "JsonVariantType.hpp"

namespace ArduinoJson {
namespace Internals {

struct JsonVariantData {
  JsonVariantType type;
  JsonVariantContent content;

  JsonVariantData() {
    type = JSON_NULL;
  }

  void setBoolean(bool value) {
    type = JSON_BOOLEAN;
    content.asInteger = static_cast<JsonUInt>(value);
  }

  void setFloat(JsonFloat value) {
    type = JSON_FLOAT;
    content.asFloat = value;
  }

  void setNegativeInteger(JsonUInt value) {
    type = JSON_NEGATIVE_INTEGER;
    content.asInteger = value;
  }

  void setPostiveInteger(JsonUInt value) {
    type = JSON_POSITIVE_INTEGER;
    content.asInteger = value;
  }

  void setOwnedString(const char *value) {
    type = JSON_OWNED_STRING;
    content.asString = value;
  }

  void setLinkedString(const char *value) {
    type = JSON_LINKED_STRING;
    content.asString = value;
  }

  void setOwnedRaw(const char *data, size_t size) {
    type = JSON_OWNED_RAW;
    content.asRaw.data = data;
    content.asRaw.size = size;
  }

  void setLinkedRaw(const char *data, size_t size) {
    type = JSON_LINKED_RAW;
    content.asRaw.data = data;
    content.asRaw.size = size;
  }

  void setNull() {
    type = JSON_NULL;
  }

  JsonArrayData *toArray() {
    type = JSON_ARRAY;
    content.asArray.head = 0;
    content.asArray.tail = 0;
    return &content.asArray;
  }

  JsonObjectData *toObject() {
    type = JSON_OBJECT;
    content.asObject.head = 0;
    content.asObject.tail = 0;
    return &content.asObject;
  }

  JsonArrayData *asArray() {
    return type == JSON_ARRAY ? &content.asArray : 0;
  }

  JsonObjectData *asObject() {
    return type == JSON_OBJECT ? &content.asObject : 0;
  }

  template <typename T>
  T asInteger() const {
    switch (type) {
      case JSON_POSITIVE_INTEGER:
      case JSON_BOOLEAN:
        return T(content.asInteger);
      case JSON_NEGATIVE_INTEGER:
        return T(~content.asInteger + 1);
      case JSON_LINKED_STRING:
      case JSON_OWNED_STRING:
        return parseInteger<T>(content.asString);
      case JSON_FLOAT:
        return T(content.asFloat);
      default:
        return 0;
    }
  }

  template <typename T>
  T asFloat() const {
    switch (type) {
      case JSON_POSITIVE_INTEGER:
      case JSON_BOOLEAN:
        return static_cast<T>(content.asInteger);
      case JSON_NEGATIVE_INTEGER:
        return -static_cast<T>(content.asInteger);
      case JSON_LINKED_STRING:
      case JSON_OWNED_STRING:
        return parseFloat<T>(content.asString);
      case JSON_FLOAT:
        return static_cast<T>(content.asFloat);
      default:
        return 0;
    }
  }

  const char *asString() const {
    return isString() ? content.asString : NULL;
  }

  bool isArray() const {
    return type == JSON_ARRAY;
  }

  bool isBoolean() const {
    return type == JSON_BOOLEAN;
  }

  bool isFloat() const {
    return type == JSON_FLOAT || type == JSON_POSITIVE_INTEGER ||
           type == JSON_NEGATIVE_INTEGER;
  }

  bool isInteger() const {
    return type == JSON_POSITIVE_INTEGER || type == JSON_NEGATIVE_INTEGER;
  }

  bool isNull() const {
    return type == JSON_NULL;
  }

  bool isObject() const {
    return type == JSON_OBJECT;
  }

  bool isString() const {
    return type == JSON_LINKED_STRING || type == JSON_OWNED_STRING;
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
