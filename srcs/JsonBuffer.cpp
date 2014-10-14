#include "JsonBuffer.h"

#include <new>
#include <string.h> // for memset

#include "JsonObject.h"
#include "JsonValue.h"
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

JsonArray JsonBuffer::parseArray(char const *string)
{
    return JsonArray();
}