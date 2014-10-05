#include "JsonArray.h"

#include "JsonValue.h"

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
    JsonNode* node = createNode(JSON_BOOLEAN);
    if (!node) return;

    node->content.asBoolean = value;
    addChild(node);
}

void JsonArray::add(char const* value)
{
    JsonNode* node = createNode(JSON_STRING);
    if (!node) return;

    node->content.asString = value;
    addChild(node);
}

void JsonArray::add(double value, int decimals)
{
    JsonNode* node = createNode((JsonNodeType)(JSON_DOUBLE_0_DECIMALS + decimals));
    if (!node) return;

    node->content.asDouble = value;
    addChild(node);
}

void JsonArray::add(long value)
{
    JsonNode* node = createNode(JSON_INTEGER);
    if (!node) return;

    node->content.asInteger = value;
    addChild(node);
}