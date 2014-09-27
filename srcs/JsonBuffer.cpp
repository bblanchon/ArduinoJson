#include "JsonBuffer.h"
#include "JsonNode.h"
#include "JsonObject.h"
#include <string.h> // for memset


JsonObject JsonBuffer::createObject()
{
    JsonNode* node = createNode(JSON_OBJECT);
    return JsonObject(this, node);
}

JsonNode* JsonBuffer::createNode(JsonNodeType type)
{
    JsonNode* node = allocateNode();
    if (!node) return 0;
    
    memset(node, 0, sizeof(JsonNode));
    node->type = type;
    return node;
}