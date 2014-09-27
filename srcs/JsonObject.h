#pragma once

class JsonValue;
struct JsonNode;

class JsonObject
{
    friend JsonValue;

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

    bool operator== (const JsonObject& other) const
    {
        return _node == other._node;
    }

private:
    JsonNode* _node;

    JsonNode* getOrCreateNodeAt(char const* key);
};