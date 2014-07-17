/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "jsmn.h"

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
            JsonValue()
                : json(0), tokens(0)
            {

            }

            JsonValue(char* json, jsmntok_t* tokens)
                : json(json), tokens(tokens)
            {

            }

            bool success()
            {
                return json != 0 && tokens != 0;
            }

            operator bool();
            operator double();
            operator long();
            operator char*();
            operator JsonArray();
            operator JsonHashTable();

            JsonValue operator[](const char*);
            JsonValue operator[](int);

            int size();

        private:

            char* json;
            jsmntok_t* tokens;

            static int getNestedTokenCount(jsmntok_t* token);
        };
    }
}