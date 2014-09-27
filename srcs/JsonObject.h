#pragma once

class JsonBuffer;
class JsonValue;
struct JsonNode;

class JsonObject
{
    //   friend class JsonValue;
    //
    //public:
    //    JsonObject(JsonBuffer& buffer, JsonNode& node)
    //        : _buffer(buffer), _node(node)
    //    {
    //    }
    //
    //    JsonObject createObject(const char* key)
    //    {
    //        JsonObject innerObject = _buffer.createObject();
    //        addNodeAt(key, innerObject._node);
    //        return innerObject;
    //    }
    //

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

    //
    //    // TODO: pull up
    //    void appendChild(JsonNode& newChild);
};