#include "JsonNode.h"

#include "JsonWriter.h"
#include "../JsonArray.h"
#include "../JsonObject.h"

void JsonNode::writeTo(JsonWriter& writer)
{
    switch (type)
    {
    case JSON_ARRAY:
        writeArrayTo(writer);
        break;

    case JSON_OBJECT:
        writeObjectTo(writer);
        break;

    case JSON_STRING:
        writer.writeValue(content.asString);
        break;

    case JSON_INTEGER:
        writer.writeValue(content.asInteger);
        break;

    case JSON_BOOLEAN:
        writer.writeValue(content.asBoolean);
        break;

    case JSON_PROXY:
        content.asProxy.target->writeTo(writer);
        break;

    default: // >= JSON_DOUBLE_0_DECIMALS
        writer.writeValue(content.asDouble, type - JSON_DOUBLE_0_DECIMALS);
        break;
    }
}

void JsonNode::writeArrayTo(JsonWriter& writer)
{
    writer.beginArray();

    JsonNode* child = content.asContainer.child;
    
    while(child)
    {
        child->writeTo(writer);

        child = child->next;
    }

    writer.endArray();
}

void JsonNode::writeObjectTo(JsonWriter& writer)
{
    writer.beginObject();

    JsonNode* child = content.asContainer.child;

    while (child)
    {
        writer.writeKey(child->content.asKey.key);
        child->content.asKey.value->writeTo(writer);

        child = child->next;
    }

    writer.endObject();
}