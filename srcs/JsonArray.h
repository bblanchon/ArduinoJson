#pragma once

#include "JsonContainer.h"

class JsonArray : public JsonContainer
{
public:
    JsonArray()
    {
    }

    explicit JsonArray(JsonNode* node)
        : JsonContainer(node)
    {
    }

    JsonValue operator[](int index) const;

    template<typename T>
    void add(T value)
    {
        JsonNode* node = createNode(JSON_UNDEFINED);
        if (!node) return;

        JsonValue jsonValue(node);
        jsonValue = value;

        addChild(node);
    }
};

