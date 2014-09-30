#pragma once

#include "Printable.h"

class JsonValue;
struct JsonNode;

class JsonObject : public Printable
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
    void remove(const char* key);

    bool operator==(const JsonObject& other) const;

    size_t printTo(char* buffer, size_t bufferSize) const;
    virtual size_t printTo(Print& print) const;

private:
    JsonNode* _node;

    JsonNode* getOrCreateNodeAt(char const* key);
};