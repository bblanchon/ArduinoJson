#include "JsonBuffer.h"

#include <string.h> // for memset

#include "JsonObject.h"
#include "JsonValue.h"
#include "Internals/JsonNode.h"

JsonValue JsonBuffer::createValue()
{
    JsonNode* node = createNode(JSON_UNDEFINED);
    return JsonValue(node);
}

JsonNode* JsonBuffer::createNode(JsonNodeType type)
{
    JsonNode* node = allocateNode();
    if (!node) return 0;
    
    memset(node, 0, sizeof(JsonNode));
    node->type = type;
    return node;
}

JsonNode* JsonBuffer::createContainerNode(JsonNodeType type)
{
    JsonNode* node = createNode(type);

    if (node)
        node->content.asContainer.buffer = this;

    return node;
}