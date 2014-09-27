#pragma once

class JsonValue;
struct JsonNode;

class JsonObject
{
public:
    JsonObject()
        : _node(0)
    {
    }

    JsonObject(JsonNode* node)
        : _node(node)
    {
    }

    size_t size();

    JsonValue operator[](const char* key);

private:
    JsonNode* _node;

    JsonNode* getOrCreateNodeAt(char const* key);
};