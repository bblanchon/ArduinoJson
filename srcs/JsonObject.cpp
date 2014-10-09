#include "JsonObject.h"

#include <string.h> // for strcmp

#include "JsonBuffer.h"
#include "JsonValue.h"
#include "Internals/EscapedString.h"
#include "Internals/JsonNode.h"
#include "Internals/StringBuilder.h"

using namespace ArduinoJson::Internals;

JsonValue JsonObject::operator[](char const* key)
{
    JsonNode* node = getOrCreateNodeAt(key);
    return JsonValue(node);
}

void JsonObject::remove(char const* key)
{
    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        const char* childKey = it->getAsObjectKey();

        if (!strcmp(childKey, key))
        {
            removeChild(*it);
        }
    }
}

JsonArray JsonObject::createNestedArray(char const* key)
{
    JsonNode* node = getOrCreateNodeAt(key);

    if (node)
        node->setAsArray(_node->getContainerBuffer());

    return JsonArray(node);
}

JsonObject JsonObject::createNestedObject(char const* key)
{
    JsonNode* node = getOrCreateNodeAt(key);
    
    if (node)
        node->setAsObject(_node->getContainerBuffer());

    return JsonObject(node);
}

JsonNode* JsonObject::getOrCreateNodeAt(const char* key)
{
    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        const char* childKey = it->getAsObjectKey();

        if (!strcmp(childKey, key))
            return it->getAsObjectValue();
    }
      
    JsonNode* newValueNode = createNode();
    if (!newValueNode) return 0;
    
    JsonNode* newKeyNode = createNode();
    if (!newKeyNode) return 0;

    newKeyNode->setAsObjectKeyValue(key, newValueNode);

    addChild(newKeyNode);

    return newValueNode;
}