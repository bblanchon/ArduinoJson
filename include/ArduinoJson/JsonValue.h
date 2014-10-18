#pragma once

#include "Internals/JsonNodeWrapper.h"

namespace ArduinoJson
{
    class JsonArray;
    class JsonContainer;
    class JsonObject;

    class JsonValue : public Internals::JsonNodeWrapper
    {
    public:

        JsonValue() {}

        explicit JsonValue(Internals::JsonNode* node)
        : JsonNodeWrapper(node)
        {
        }

        void operator=(bool);
        void operator=(const char*);
        void operator=(double x) { set(x, 2); }
        void operator=(int);
        void operator=(const JsonValue& value) { duplicate(value); }
        void operator=(const Internals::JsonNodeWrapper& object) { duplicate(object); }
        
        operator bool() const;
        operator const char*() const;
        operator double() const;
        operator long() const;
        operator int() const { return operator long(); }
        operator JsonArray() const;
        operator JsonObject() const;

        void set(double value, int decimals);
    };
}