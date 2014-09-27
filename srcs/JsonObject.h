#pragma once

class JsonBuffer;
struct JsonNode;
class JsonValue;

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
//    JsonValue& operator[](const char* key);

public:

    int size()
    {
        return 0;
    }

//private:
//    JsonBuffer& _buffer;
//    JsonNode& _node;
//
//    void addNodeAt(char const* key, JsonNode& node);
//    JsonNode& getNodeAt(const char* key);
//
//    // TODO: pull up
//    void appendChild(JsonNode& newChild);
};