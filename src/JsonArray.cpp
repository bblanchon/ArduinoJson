#include "ArduinoJson/JsonArray.hpp"
#include "ArduinoJson/JsonObject.hpp"
#include "ArduinoJson/JsonValue.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonValue JsonArray::operator[](int index) const
{
    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        if (!index) return JsonValue(*it);
        index--;
    }

    return JsonValue();
}

void JsonArray::add(bool value)
{
    JsonNode* node = createNode();
    if (!node) return;

    node->setAsBoolean(value);
    addChild(node);
}

void JsonArray::add(char const* value)
{
    JsonNode* node = createNode();
    if (!node) return;

    node->setAsString(value);
    addChild(node);
}

void JsonArray::add(double value, int decimals)
{
    JsonNode* node = createNode();
    if (!node) return;

    node->setAsDouble(value, decimals);
    addChild(node);
}

void JsonArray::add(long value)
{
    JsonNode* node = createNode();
    if (!node) return;

    node->setAsLong(value);
    addChild(node);
}

// TODO: we should have the same issue as in JsonValue
void JsonArray::add(JsonContainer nestedContainer)
{
    JsonNode* node = createNode();
    if (!node) return;

    node->duplicate(nestedContainer._node);
    addChild(node);
}

JsonArray JsonArray::createNestedArray()
{
    JsonNode* node = createNode();
    
    if (node)
    {
        node->setAsArray(_node->getContainerBuffer());
        addChild(node);
    }
    
    return JsonArray(node);
}

JsonObject JsonArray::createNestedObject()
{
    JsonNode* node = createNode();
    
    if (node)
    {
        node->setAsObject(_node->getContainerBuffer());
        addChild(node);
    }
    
    return JsonObject(node);
}