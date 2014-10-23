#include "ArduinoJson/JsonBuffer.hpp"

#include <new>

#include "ArduinoJson/JsonValue.hpp"
#include "ArduinoJson/Internals/JsonParser.hpp"
#include "ArduinoJson/Internals/JsonNode.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValue JsonBuffer::createValue() { return JsonValue(createNode()); }

JsonNode *JsonBuffer::createNode() {
  void *node = allocateNode();
  if (!node)
    return 0;

  return new (node) JsonNode();
}

JsonArray JsonBuffer::parseArray(char *json) {
  JsonParser parser(this, json);
  return JsonArray(parser.parseAnything());
}

JsonObject JsonBuffer::parseObject(char *json) {
  JsonParser parser(this, json);
  return JsonObject(parser.parseAnything());
}

JsonValue JsonBuffer::parseValue(char *json) {
  JsonParser parser(this, json);
  return JsonValue(parser.parseAnything());
}

JsonNode *JsonBuffer::createArrayNode() {
  JsonNode *node = createNode();

  if (node)
    node->setAsArray(this);

  return node;
}

JsonNode *JsonBuffer::createBoolNode(bool value) {
  JsonNode *node = createNode();

  if (node)
    node->setAsBoolean(value);

  return node;
}

JsonNode *JsonBuffer::createDoubleNode(double value, int decimals) {
  JsonNode *node = createNode();

  if (node)
    node->setAsDouble(value, decimals);

  return node;
}

JsonNode *JsonBuffer::createLongNode(long value) {
  JsonNode *node = createNode();

  if (node)
    node->setAsLong(value);

  return node;
}

JsonNode *JsonBuffer::createObjectNode() {
  JsonNode *node = createNode();

  if (node)
    node->setAsObject(this);

  return node;
}

Internals::JsonNode *JsonBuffer::createObjectKeyValueNode(const char *key,
                                                          JsonNode *value) {
  JsonNode *node = createNode();

  if (node)
    node->setAsObjectKeyValue(key, value);

  return node;
}

JsonNode *JsonBuffer::createStringNode(const char *value) {
  JsonNode *node = createNode();

  if (node)
    node->setAsString(value);

  return node;
}