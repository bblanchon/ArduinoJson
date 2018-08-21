// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"

namespace ArduinoJson {

namespace Internals {

struct JsonPairData {
  const char* key;
  JsonVariantData value;
};
}  // namespace Internals

// A key value pair for JsonObjectData.
class JsonPair {
 public:
  JsonPair(Internals::JsonBuffer* buffer, Internals::JsonPairData* data)
      : _key(data->key), _value(buffer, &data->value) {}

  const char* key() const {
    return _key;
  }

  JsonVariant value() const {
    return _value;
  }

 private:
  const char* _key;
  JsonVariant _value;
};
}  // namespace ArduinoJson
