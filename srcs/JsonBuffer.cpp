#include "JsonBuffer.h"

#include <new>

#include "JsonValue.h"
#include "Internals/JsonParser.h"
#include "Internals/JsonNode.h"

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