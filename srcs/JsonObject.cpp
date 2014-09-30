#include "JsonObject.h"

#include <string.h> // for strcmp

#include "EscapedString.h"
#include "JsonBuffer.h"
#include "JsonValue.h"
#include "JsonNode.h"
#include "StringBuilder.h"

using namespace ArduinoJson::Internals;

size_t JsonObject::size()
{
    JsonNode* firstChild = _node->content.asObject.child;

    int size = 0;

    for (JsonNode* child = firstChild; child; child = child->next)
    {
        size++;
    }

    return size;
}

JsonValue JsonObject::operator[](char const* key)
{
    JsonNode* node = getOrCreateNodeAt(key);
    return JsonValue(node);
}

void JsonObject::remove(char const* key)
{
    JsonNode* firstChild = _node->content.asObject.child;
    JsonNode* lastChild = 0;

    for (JsonNode* child = firstChild; child; child = child->next)
    {
        const char* childKey = child->content.asKey.key;

        if (!strcmp(childKey, key))
        {
            if (lastChild)
                lastChild->next = child->next;
            else
                _node->content.asObject.child = child->next;
        }       
        lastChild = child;
    }
}

bool JsonObject::operator==(JsonObject const& other) const
{
    return _node == other._node;
}

JsonNode* JsonObject::getOrCreateNodeAt(char const* key)
{
    if (!_node || _node->type != JSON_OBJECT) return 0;

    JsonNode* firstChild = _node->content.asObject.child;
    JsonNode* lastChild = 0;

    for (JsonNode* child = firstChild; child; child = child->next)
    {
        const char* childKey = child->content.asKey.key;

        if (!strcmp(childKey, key))
            return child->content.asKey.value;

        lastChild = child;
    }

    JsonBuffer* buffer = _node->content.asObject.buffer;

    JsonNode* newValueNode = buffer->createNode(JSON_UNDEFINED);
    if (!newValueNode) return 0;
    
    JsonNode* newKeyNode = buffer->createNode(JSON_KEY);
    if (!newKeyNode) return 0;

    newKeyNode->content.asKey.key = key;
    newKeyNode->content.asKey.value = newValueNode;

    if (lastChild)
        lastChild->next = newKeyNode;
    else
        _node->content.asObject.child = newKeyNode;

    return newValueNode;
}

size_t JsonObject::printTo(char* buffer, size_t bufferSize) const
{
    StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
}

size_t JsonObject::printTo(Print& p) const
{
    size_t n = 0;

    n += p.write('{');

    JsonNode* firstChild = _node->content.asObject.child;

    for (JsonNode* child = firstChild; child; child = child->next)
    {
        const char* childKey = child->content.asKey.key;
        JsonNode* childValue = child->content.asKey.value;

        n += EscapedString::printTo(childKey, p);
        n += p.write(':');

        switch (childValue->type)
        {
        case JSON_STRING:
            n += EscapedString::printTo(childValue->content.asString, p);
            break;

        case JSON_INTEGER:
            n += p.print(childValue->content.asInteger);
            break;

        case JSON_BOOLEAN:
            n += p.print(childValue->content.asBoolean ? "true" : "false");
            break;
        }

        if (childValue->type >= JSON_DOUBLE_0_DECIMALS)
            n += p.print(childValue->content.asDouble, childValue->type - JSON_DOUBLE_0_DECIMALS);

        if (child->next)
        {
            n += p.write(',');
        }
    }

    n += p.write('}');

    return n;
}