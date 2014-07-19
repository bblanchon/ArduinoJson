/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonToken.h"

#ifndef ARDUINO_JSON_NO_DEPRECATED_WARNING
#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#endif
#else
#define DEPRECATED
#endif

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonArray;
        class JsonObject;

        class JsonValue : protected JsonToken
        {
        public:

            JsonValue()
            {

            }

            JsonValue(JsonToken token)
                : JsonToken(token)
            {

            }

            bool success()
            {
                return isValid();
            }
            
            operator bool();
            operator double();
            operator long();
            operator char*();
            JsonValue operator[](int index);
            JsonValue operator[](const char*key);
        };
    }
}