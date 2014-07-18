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
        class JsonHashTable;

        class JsonValue
        {
        public:

            JsonValue(char* json, Internal::JsonToken token)
                : json(json), token(token)
            {

            }

            bool success()
            {
                return token.isValid();
            }
            
            operator bool();
            operator double();
            operator long();
            operator char*();
            operator JsonArray();
            operator JsonHashTable();
            JsonValue operator[](int index);
            JsonValue operator[](const char*key);

            static JsonValue null()
            {
                return JsonValue(0, Internal::JsonToken(0));
            }

        private:

            char* json;
            Internal::JsonToken token;
        };
    }
}