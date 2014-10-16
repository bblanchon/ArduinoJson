#include "ArduinoJson/JsonBuffer.h"

#include <new>

#include "ArduinoJson/JsonValue.h"
#include "ArduinoJson/Internals/JsonParser.h"
#include "ArduinoJson/Internals/JsonNode.h"

JsonValue JsonBuffer::createValue()
{
    return JsonValue(createNode());
}

JsonNode* JsonBuffer::createNode()
{
    void* node = allocateNode();
    if (!node) return 0;
        
    return new (node) JsonNode();
}

JsonArray JsonBuffer::parseArray(char* json)
{
    JsonParser parser(this, json);
    return JsonArray(parser.parseAnything());
}

JsonNode* JsonBuffer::createArrayNode()
{
    JsonNode* node = createNode();

    if (node)
        node->setAsArray(this);

    return node;
}

JsonNode* JsonBuffer::createBoolNode(bool value)
{
    JsonNode* node = createNode();

    if (node)
        node->setAsBoolean(value);

    return node;
}

JsonNode* JsonBuffer::createDoubleNode(double value, int decimals)
{
    JsonNode* node = createNode();

    if (node)
        node->setAsDouble(value, decimals);

    return node;
}

JsonNode* JsonBuffer::createLongNode(long value)
{
    JsonNode* node = createNode();

    if (node)
        node->setAsLong(value);

    return node;
}

JsonNode* JsonBuffer::createObjectNode()
{
    JsonNode* node = createNode();

    if (node)
        node->setAsObject(this);

    return node;
}

JsonNode* JsonBuffer::createStringNode(const char* value)
{
    JsonNode* node = createNode();

    if (node)
        node->setAsString(value);

    return node;
}