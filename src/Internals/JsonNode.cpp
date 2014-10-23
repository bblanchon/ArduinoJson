#include "ArduinoJson/Internals/JsonNode.hpp"

#include "ArduinoJson/Internals/JsonWriter.hpp"
#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonBuffer.hpp"

using namespace ArduinoJson::Internals;

void JsonNode::writeTo(JsonWriter &writer) {
  switch (type) {
  case JSON_PROXY:
    content.asProxy.target->writeTo(writer);
    break;

  case JSON_ARRAY:
    writeArrayTo(writer);
    break;

  case JSON_OBJECT:
    writeObjectTo(writer);
    break;

  case JSON_STRING:
    writer.writeString(content.asString);
    break;

  case JSON_LONG:
    writer.writeInteger(content.asInteger);
    break;

  case JSON_BOOLEAN:
    writer.writeBoolean(content.asBoolean);
    break;

  default: // >= JSON_DOUBLE_0_DECIMALS
    writer.writeDouble(content.asDouble, type - JSON_DOUBLE_0_DECIMALS);
    break;
  }
}

void JsonNode::addChild(JsonNode *childToAdd) {
  if (type == JSON_PROXY)
    return content.asProxy.target->addChild(childToAdd);

  if (type != JSON_ARRAY && type != JSON_OBJECT)
    return;

  JsonNode *lastChild = content.asContainer.child;

  if (!lastChild) {
    content.asContainer.child = childToAdd;
    return;
  }

  while (lastChild->next)
    lastChild = lastChild->next;

  lastChild->next = childToAdd;
}

void JsonNode::removeChild(JsonNode *childToRemove) {
  if (type == JSON_PROXY)
    return content.asProxy.target->removeChild(childToRemove);

  if (type != JSON_ARRAY && type != JSON_OBJECT)
    return;

  if (content.asContainer.child == childToRemove) {
    content.asContainer.child = childToRemove->next;
    return;
  }

  for (JsonNode *child = content.asContainer.child; child;
       child = child->next) {
    if (child->next == childToRemove)
      child->next = childToRemove->next;
  }
}

void JsonNode::writeArrayTo(JsonWriter &writer) {
  JsonNode *child = content.asContainer.child;

  if (child) {
    writer.beginArray();

    for (;;) {
      child->writeTo(writer);

      child = child->next;
      if (!child)
        break;

      writer.writeComma();
    }

    writer.endArray();
  } else {
    writer.writeEmptyArray();
  }
}

void JsonNode::writeObjectTo(JsonWriter &writer) {
  JsonNode *child = content.asContainer.child;

  if (child) {
    writer.beginObject();

    for (;;) {
      writer.writeString(child->content.asKeyValue.key);
      writer.writeColon();
      child->content.asKeyValue.value->writeTo(writer);

      child = child->next;
      if (!child)
        break;

      writer.writeComma();
    }

    writer.endObject();
  } else {
    writer.writeEmptyObject();
  }
}

void JsonNode::setAsProxyOfSelf() {
  JsonBuffer *buffer = content.asContainer.buffer;
  if (!buffer)
    return;

  JsonNode *newNode = buffer->createNode();
  if (!newNode)
    return;

  *newNode = *this;

  setAsProxyOf(newNode);
}

void JsonNode::duplicate(JsonNode *other) {
  if (!other) {
    type = JSON_UNDEFINED;
  } else if (other->type == JSON_ARRAY || other->type == JSON_OBJECT) {
    other->setAsProxyOfSelf();
    setAsProxyOf(other->content.asProxy.target);
  } else {
    *this = *other;
  }
}
