#pragma once

#include "JsonArray.h"
#include "JsonObject.h"

class JsonBuffer
{
    friend class JsonContainer;

public:
    virtual ~JsonBuffer() {};

    JsonArray createArray()
    {
        return JsonArray(createContainerNode(JSON_ARRAY));
    }

    JsonObject createObject()
    {
        return JsonObject(createContainerNode(JSON_OBJECT));
    }

    JsonValue createValue();

protected:
    virtual JsonNode* allocateNode() = 0;

private:
    JsonNode* createNode(JsonNodeType type);
    JsonNode* createContainerNode(JsonNodeType type);
};

