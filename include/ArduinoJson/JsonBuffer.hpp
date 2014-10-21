#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"

namespace ArduinoJson
{
    namespace Internals
    {
        class JsonParser;
    }

    class JsonBuffer
    {
        friend class JsonContainer;
        friend class Internals::JsonNode;
        friend class Internals::JsonParser;

    public:
        virtual ~JsonBuffer() {};

        JsonArray createArray()
        {
            return JsonArray(createArrayNode());
        }

        JsonObject createObject()
        {
            return JsonObject(createObjectNode());
        }

        JsonValue createValue();

        JsonArray parseArray(char* json);
        JsonObject parseObject(char* json);
        JsonValue parseValue(char* json); // TODO: remove

    protected:
        virtual void* allocateNode() = 0;

    private:
        Internals::JsonNode* createNode();

        Internals::JsonNode* createArrayNode();
        Internals::JsonNode* createBoolNode(bool value);
        Internals::JsonNode* createDoubleNode(double value, int decimals);
        Internals::JsonNode* createLongNode(long value);
        Internals::JsonNode* createObjectNode();
        Internals::JsonNode* createObjectKeyValueNode(const char* key, Internals::JsonNode* value);
        Internals::JsonNode* createStringNode(const char* value);
    };
}