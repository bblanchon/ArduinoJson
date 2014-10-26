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

  operator bool() const {
    return _type == JSON_BOOLEAN ? _content.asBoolean : false;
  }

  operator char const *() const {
    return _type == JSON_STRING ? _content.asString : NULL;
  }

  operator double() const {
    return _type >= JSON_DOUBLE_0_DECIMALS ? _content.asDouble : 0;
  }

  operator long() const { return _type == JSON_LONG ? _content.asInteger : 0; }

  operator JsonArrayImpl *() const {
    return _type == JSON_ARRAY ? _content.asArray : NULL;
  }

  operator JsonObjectImpl *() const {
    return _type == JSON_OBJECT ? _content.asObject : NULL;
  }

 private:
  Internals::JsonValueType _type;
  Internals::JsonValueContent _content;
};
}
}
