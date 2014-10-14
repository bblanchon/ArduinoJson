#include "JsonBuffer.h"

#include <new>

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

JsonArray JsonBuffer::parseArray(const char* json)
{
    JsonNode* root;

    while(*json == ' ') json++;

    if (json[0] == '[')
    {
        root = createNode();
        root->setAsArray(this);
    }
    else
    {
        root = 0;
    }

    return JsonArray(root);
}