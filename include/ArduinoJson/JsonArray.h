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

    void add(bool value);
    void add(const char* value);
    void add(double value, int decimals=2);
    void add(int value) { add((long) value); }
    void add(long value);
    void add(JsonContainer nestedArray); // TODO: should allow JsonValue too

    JsonArray createNestedArray();
    JsonObject createNestedObject();

    bool success()
    {
        return _node && _node->isArray();
    }
};

