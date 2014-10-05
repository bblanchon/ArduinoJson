#include "JsonObject.h"

#include <string.h> // for strcmp

#include "JsonBuffer.h"
#include "JsonValue.h"
#include "Internals/EscapedString.h"
#include "Internals/JsonNode.h"
#include "Internals/JsonNodeSerializer.h"
#include "Internals/StringBuilder.h"

using namespace ArduinoJson::Internals;

JsonValue JsonObject::operator[](char const* key)
{
    JsonNode* node = getOrCreateNodeAt(key);
    return JsonValue(node);
}

void JsonObject::remove(char const* key)
{
    JsonNode* lastChild = 0;

    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        const char* childKey = it->content.asKey.key;

        if (!strcmp(childKey, key))
        {
            removeChildAfter(*it, lastChild);
        }      

        lastChild = *it;
    }
}

JsonNode* JsonObject::getOrCreateNodeAt(const char* key)
{
    if (!checkNodeType(JSON_OBJECT)) return 0;

    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        const char* childKey = it->content.asKey.key;

        if (!strcmp(childKey, key))
            return it->content.asKey.value;
    }
      
    JsonNode* newValueNode = createNode(JSON_UNDEFINED);
    if (!newValueNode) return 0;
    
    JsonNode* newKeyNode = createNode(JSON_KEY);
    if (!newKeyNode) return 0;

    newKeyNode->content.asKey.key = key;
    newKeyNode->content.asKey.value = newValueNode;

    addChild(newKeyNode);

    return newValueNode;
}
