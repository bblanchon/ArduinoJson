#pragma once

#include <stddef.h>
#include "JsonNode.h"

class JsonObject;
struct JsonNode;

class JsonBuffer
{
    friend class JsonObject;

public:
//    virtual ~JsonBuffer() = 0;

    JsonObject createObject();
protected:
    virtual JsonNode* allocateNode() = 0;

private:
    JsonNode* createNode(JsonNodeType type);
};

