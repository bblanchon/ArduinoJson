#include "JsonNodeSerializer.h"

#include "EscapedString.h"
#include "JsonNode.h"

using namespace ArduinoJson::Internals;

size_t JsonNodeSerializer::serialize(const JsonNode* node)
{
    if (!node) return 0;

    switch (node->type)
    {
    case JSON_OBJECT:
        return serializeObject(node);

    case JSON_STRING:
        return EscapedString::printTo(node->content.asString, _sink);

    case JSON_INTEGER:
        return _sink.print(node->content.asInteger);

    case JSON_BOOLEAN:
        return _sink.print(node->content.asBoolean ? "true" : "false");

    case JSON_PROXY:
        return serialize(node->content.asProxy.target);
    }

    if (node->type >= JSON_DOUBLE_0_DECIMALS)
    {
        return _sink.print(node->content.asDouble, node->type - JSON_DOUBLE_0_DECIMALS);
    }

    return 0;
}

size_t JsonNodeSerializer::serializeObject(const JsonNode* node)
{
    size_t n = 0;

    n += _sink.write('{');

    JsonNode* firstChild = node->content.asObject.child;

    for (JsonNode* child = firstChild; child; child = child->next)
    {
        n += serializeKeyValue(child);
        
        if (child->next)
        {
            n += _sink.write(',');
        }
    }

    n += _sink.write('}');

    return n;
}

size_t JsonNodeSerializer::serializeKeyValue(JsonNode const* node)
{
    const char* childKey = node->content.asKey.key;
    JsonNode* childValue = node->content.asKey.value;

    return
        EscapedString::printTo(childKey, _sink) +
        _sink.write(':') +
        serialize(childValue);
}