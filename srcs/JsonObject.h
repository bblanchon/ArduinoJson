#pragma once

class JsonBuffer;
class JsonValue;
struct JsonNode;

class JsonObject
{
public:

    JsonObject(JsonBuffer* buffer, JsonNode* node)
        : _buffer(buffer), _node(node)
    {
    }

    size_t size();

    JsonValue operator[](const char* key);
private:

    JsonBuffer* _buffer;
    JsonNode* _node;
    JsonNode* getOrCreateNodeAt(char const* key);
};