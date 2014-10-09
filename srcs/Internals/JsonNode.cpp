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
        writer.writeString(content.asString);
        break;

    case JSON_LONG:
        writer.writeInteger(content.asInteger);
        break;

    case JSON_BOOLEAN:
        writer.writeBoolean(content.asBoolean);
        break;

    default: // >= JSON_DOUBLE_0_DECIMALS
        writer.writeDouble(content.asDouble, type - JSON_DOUBLE_0_DECIMALS);
        break;
    }
}

void JsonNode::addChildToContainer(JsonNode* childToAdd)
{
    if (type != JSON_ARRAY && type != JSON_OBJECT) return;

    JsonNode* lastChild = content.asContainer.child;

    if (!lastChild)
    {
        content.asContainer.child = childToAdd;
        return;
    }

    while (lastChild->next)
        lastChild = lastChild->next;

    lastChild->next = childToAdd;
}

void JsonNode::removeChildFromContainer(JsonNode* childToRemove)
{
    if (type != JSON_ARRAY && type != JSON_OBJECT) return;

    if (content.asContainer.child == childToRemove)
    {
        content.asContainer.child = childToRemove->next;
        return;
    }

    for (JsonNode* child = content.asContainer.child; child; child = child->next)
    {
        if (child->next == childToRemove)
            child->next = childToRemove->next;
    }
}

void JsonNode::writeArrayTo(JsonWriter& writer)
{
    JsonNode* child = content.asContainer.child;

    if (child)
    {
        writer.beginArray();

        while (true)
        {
            child->writeTo(writer);

            child = child->next;
            if (!child) break;
            
            writer.writeComma();
        } 

        writer.endArray();
    }
    else
    {
        writer.writeEmptyArray();
    }
}

void JsonNode::writeObjectTo(JsonWriter& writer)
{
    JsonNode* child = content.asContainer.child;

    if (child)
    {
        writer.beginObject();

        while (true)
        {
            writer.writeString(child->content.asKey.key);
            writer.writeColon();
            child->content.asKey.value->writeTo(writer);

            child = child->next;
            if (!child) break;

            writer.writeComma();
        }

        writer.endObject();
    }
    else
    {
        writer.writeEmptyObject();
    }
}