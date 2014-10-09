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
        JsonNode* node = createNode();
        if (node) node->setAsArray(this);
        return JsonArray(node);
    }

    JsonObject createObject()
    {
        JsonNode* node = createNode();
        if (node) node->setAsObject(this);
        return JsonObject(node);
    }

    JsonValue createValue();

protected:
    virtual JsonNode* allocateNode() = 0;

private:
    JsonNode* createNode(JsonNodeType type = JSON_UNDEFINED);
};

