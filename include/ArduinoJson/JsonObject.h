#pragma once

#include "JsonContainer.h"

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

    private:
        Internals::JsonNode* getOrCreateNodeAt(const char* key);
    };
}