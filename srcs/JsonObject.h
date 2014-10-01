#pragma once

#include "JsonContainer.h"

class JsonValue;
struct JsonNode;

class JsonObject : public JsonContainer
{
    friend JsonValue;

public:
    JsonObject()
    {
    }

    explicit JsonObject(JsonNode* node)
        : JsonContainer(node)
    {
    }

    size_t size();

    JsonValue operator[](const char* key);
    void remove(const char* key);

    bool operator==(const JsonObject& other) const;

private:
    JsonNode* getOrCreateNodeAt(char const* key);
};