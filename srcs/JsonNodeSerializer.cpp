#include "JsonNodeSerializer.h"

#include "EscapedString.h"
#include "JsonNode.h"

using namespace ArduinoJson::Internals;

size_t JsonNodeSerializer::serialize(JsonNode const* node)
{
    size_t n = 0;

    n += _sink.write('{');

    JsonNode* firstChild = node->content.asObject.child;

    for (JsonNode* child = firstChild; child; child = child->next)
    {
        const char* childKey = child->content.asKey.key;
        JsonNode* childValue = child->content.asKey.value;

        n += EscapedString::printTo(childKey, _sink);
        n += _sink.write(':');

        switch (childValue->type)
        {
        case JSON_STRING:
            n += EscapedString::printTo(childValue->content.asString, _sink);
            break;

        case JSON_INTEGER:
            n += _sink.print(childValue->content.asInteger);
            break;

        case JSON_BOOLEAN:
            n += _sink.print(childValue->content.asBoolean ? "true" : "false");
            break;
        }

        if (childValue->type >= JSON_DOUBLE_0_DECIMALS)
            n += _sink.print(childValue->content.asDouble, childValue->type - JSON_DOUBLE_0_DECIMALS);

        if (child->next)
        {
            n += _sink.write(',');
        }
    }

    n += _sink.write('}');

    return n;
}