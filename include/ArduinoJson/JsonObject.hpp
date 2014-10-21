#pragma once

#include "JsonContainer.hpp"

namespace ArduinoJson
{
    class JsonObject : public JsonContainer
    {
    public:

        JsonObject()
        {
        }

        explicit JsonObject(Internals::JsonNode* node)
            : JsonContainer(node)
        {
        }

        JsonValue operator[](const char* key);
        void remove(const char* key);

        JsonArray createNestedArray(const char* key);
        JsonObject createNestedObject(const char* key);

        bool success()
        {
            return _node && _node->isObject();
        }

    private:
        Internals::JsonNode* getOrCreateNodeAt(const char* key);
    };
}