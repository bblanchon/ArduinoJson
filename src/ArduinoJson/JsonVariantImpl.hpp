// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonArrayData.hpp"
#include "JsonObjectData.hpp"
#include "JsonVariant.hpp"
#include "Numbers/isFloat.hpp"
#include "Numbers/isInteger.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"

#include <string.h>  // for strcmp

namespace ArduinoJson {

inline JsonVariant::JsonVariant(JsonArray array) {
  if (!array.isNull()) {
    _type = Internals::JSON_ARRAY;
    _content.asArray = array._data;
  } else {
    _type = Internals::JSON_UNDEFINED;
  }
}

inline JsonVariant::JsonVariant(JsonObject object) {
  if (!object.isNull()) {
    _type = Internals::JSON_OBJECT;
    _content.asObject = object._data;
  } else {
    _type = Internals::JSON_UNDEFINED;
  }
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonArray>::value,
    JsonArray>::type
JsonVariant::as() const {
  return variantAsArray();
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonObject>::value,
    T>::type
JsonVariant::as() const {
  return variantAsObject();
}

inline JsonArray JsonVariant::variantAsArray() const {
  if (_type == Internals::JSON_ARRAY) return _content.asArray;
  return JsonArray();
}

inline JsonObject JsonVariant::variantAsObject() const {
  if (_type == Internals::JSON_OBJECT) return _content.asObject;
  return JsonObject();
}

template <typename T>
inline T JsonVariant::variantAsInteger() const {
  using namespace Internals;
  switch (_type) {
    case JSON_UNDEFINED:
    case JSON_UNPARSED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return T(_content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return T(~_content.asInteger + 1);
    case JSON_STRING:
      return parseInteger<T>(_content.asString);
    default:
      return T(_content.asFloat);
  }
}

inline const char *JsonVariant::variantAsString() const {
  using namespace Internals;
  return _type == JSON_STRING ? _content.asString : NULL;
}

template <typename T>
inline T JsonVariant::variantAsFloat() const {
  using namespace Internals;
  switch (_type) {
    case JSON_UNDEFINED:
    case JSON_UNPARSED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return static_cast<T>(_content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return -static_cast<T>(_content.asInteger);
    case JSON_STRING:
      return parseFloat<T>(_content.asString);
    default:
      return static_cast<T>(_content.asFloat);
  }
}

inline bool JsonVariant::variantIsBoolean() const {
  using namespace Internals;
  return _type == JSON_BOOLEAN;
}

inline bool JsonVariant::variantIsInteger() const {
  using namespace Internals;

  return _type == JSON_POSITIVE_INTEGER || _type == JSON_NEGATIVE_INTEGER;
}

inline bool JsonVariant::variantIsFloat() const {
  using namespace Internals;

  return _type == JSON_FLOAT || _type == JSON_POSITIVE_INTEGER ||
         _type == JSON_NEGATIVE_INTEGER;
}

}  // namespace ArduinoJson
