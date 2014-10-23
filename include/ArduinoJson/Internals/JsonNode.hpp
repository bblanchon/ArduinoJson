// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

namespace ArduinoJson {
class JsonBuffer;

namespace Internals {
class JsonWriter;

class JsonNode {
  enum JsonNodeType {
    JSON_UNDEFINED,
    JSON_NULL,
    JSON_ARRAY,
    JSON_OBJECT,
    JSON_KEY_VALUE,
    JSON_BOOLEAN,
    JSON_STRING,
    JSON_LONG,
    JSON_PROXY,
    JSON_DOUBLE_0_DECIMALS,
    JSON_DOUBLE_1_DECIMAL,
    JSON_DOUBLE_2_DECIMALS
    // etc.
  };

  union JsonNodeContent {
    bool asBoolean;
    double asDouble;
    long asInteger;
    const char *asString;

    struct {
      const char *key;
      JsonNode *value;
    } asKeyValue;

    struct {
      JsonNode *child;
      JsonBuffer *buffer;
    } asContainer;

    struct {
      JsonNode *target;
    } asProxy;
  };

 public:
  JsonNode() : next(0), type(JSON_UNDEFINED) {}

  JsonNode *next;

  void writeTo(JsonWriter &);  // TODO: <- move in JsonNodeSerializer

  void setAsArray(JsonBuffer *buffer) {
    type = JSON_ARRAY;
    content.asContainer.child = 0;
    content.asContainer.buffer = buffer;
  }

  void setAsBoolean(bool value) {
    type = JSON_BOOLEAN;
    content.asBoolean = value;
  }

  void setAsLong(int value) {
    type = JSON_LONG;
    content.asInteger = value;
  }

  void setAsString(char const *value) {
    type = JSON_STRING;
    content.asString = value;
  }

  void setAsDouble(double value, int decimals) {
    type = static_cast<JsonNodeType>(JSON_DOUBLE_0_DECIMALS + decimals);
    content.asDouble = value;
  }

  void setAsObject(JsonBuffer *buffer) {
    type = JSON_OBJECT;
    content.asContainer.child = 0;
    content.asContainer.buffer = buffer;
  }

  void setAsObjectKeyValue(const char *key, JsonNode *value) {
    type = JSON_KEY_VALUE;
    content.asKeyValue.key = key;
    content.asKeyValue.value = value;
  }

  bool getAsBoolean() {
    return type == JSON_BOOLEAN ? content.asBoolean : false;
  }

  double getAsDouble() {
    return type >= JSON_DOUBLE_0_DECIMALS ? content.asDouble : 0;
  }

  long getAsInteger() { return type == JSON_LONG ? content.asInteger : 0; }

  const char *getAsString() {
    return type == JSON_STRING ? content.asString : 0;
  }

  JsonBuffer *getContainerBuffer() {
    if (type == JSON_PROXY) return content.asProxy.target->getContainerBuffer();
    return type == JSON_ARRAY || type == JSON_OBJECT
               ? content.asContainer.buffer
               : 0;
  }

  JsonNode *getContainerChild() {
    if (type == JSON_PROXY) return content.asProxy.target->getContainerChild();
    return type == JSON_ARRAY || type == JSON_OBJECT ? content.asContainer.child
                                                     : 0;
  }

  const char *getAsObjectKey() {
    return type == JSON_KEY_VALUE ? content.asKeyValue.key : 0;
  }

  JsonNode *getAsObjectValue() {
    return type == JSON_KEY_VALUE ? content.asKeyValue.value : 0;
  }

  JsonNode *getProxyTarget() {
    return type == JSON_PROXY ? content.asProxy.target : this;
  }

  bool isArray() { return type == JSON_ARRAY; }

  bool isObject() { return type == JSON_OBJECT; }

  void addChild(JsonNode *childToAdd);

  void removeChild(JsonNode *childToRemove);

  void duplicate(JsonNode *other);

 private:
  JsonNodeType type;
  JsonNodeContent content;

  inline void writeArrayTo(
      JsonWriter &);  // TODO: <- move in JsonNodeSerializer
  inline void writeObjectTo(
      JsonWriter &);  // TODO: <- move in JsonNodeSerializer

  void setAsProxyOfSelf();

  void setAsProxyOf(JsonNode *target) {
    type = JSON_PROXY;
    content.asProxy.target = target;
  }
};
}
}
