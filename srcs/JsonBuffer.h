#pragma once

#include "JsonArray.h"
#include "JsonObject.h"

class JsonParser;

class JsonBuffer
{
    friend class JsonContainer;
    friend class JsonNode;
    friend class JsonParser;

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

    JsonArray parseArray(char* string);

protected:
    virtual void* allocateNode() = 0;

private:
    JsonNode* createNode();
};

