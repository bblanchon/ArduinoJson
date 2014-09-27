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


    explicit JsonObject()
        : _size(0)
    {
    }

    int size()
    {
        return _size;
    }

    JsonValue operator[](const char* key);

private:

    int _size;

    //    JsonBuffer& _buffer;
    //    JsonNode& _node;
    //
    //    void addNodeAt(char const* key, JsonNode& node);
    //    JsonNode& getNodeAt(const char* key);
    //
    //    // TODO: pull up
    //    void appendChild(JsonNode& newChild);
};