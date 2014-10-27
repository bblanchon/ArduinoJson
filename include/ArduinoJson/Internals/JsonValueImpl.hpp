// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include <stddef.h>

#include "../ForwardDeclarations.hpp"
#include "JsonValueContent.hpp"
#include "JsonValueType.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonValueImpl {
 public:
  JsonValueImpl() : _type(JSON_UNDEFINED) {}

  void set(bool value) {
    _type = JSON_BOOLEAN;
    _content.asBoolean = value;
  }

  void set(const char *value) {
    _type = JSON_STRING;
    _content.asString = value;
  }

  void set(double value, int decimals = 2) {
    _type = static_cast<JsonValueType>(JSON_DOUBLE_0_DECIMALS + decimals);
    _content.asDouble = value;
  }

  void set(long value) {
    _type = JSON_LONG;
    _content.asInteger = value;
  }

  void set(JsonArrayImpl *array) {
    _type = JSON_ARRAY;
    _content.asArray = array;
  }

  void set(JsonObjectImpl *object) {
    _type = JSON_OBJECT;
    _content.asObject = object;
  }

  JsonArrayImpl *asArray() {
    return _type == JSON_ARRAY ? _content.asArray : NULL;
  }

  JsonObjectImpl *asObject() {
    return _type == JSON_OBJECT ? _content.asObject : NULL;
  }

  bool asBool() const {
    return _type == JSON_BOOLEAN ? _content.asBoolean : false;
  }

  const char *asString() const {
    return _type == JSON_STRING ? _content.asString : NULL;
  }

  double asDouble() const {
    return _type >= JSON_DOUBLE_0_DECIMALS ? _content.asDouble : 0;
  }

  long asLong() const { return _type == JSON_LONG ? _content.asInteger : 0; }

  void writeTo(JsonWriter &writer) const;

 private:
  Internals::JsonValueType _type;
  Internals::JsonValueContent _content;
};
}
}
