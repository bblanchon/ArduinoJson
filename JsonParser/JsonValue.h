/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "jsmn.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonArray;
        class JsonHashTable;

        class JsonValue
        {
            friend JsonArray;
            friend JsonHashTable;

        public:
            bool success()
            {
                return token != 0;
            }

            operator bool();
            operator double();
            operator long();
            operator char*();
            operator JsonArray();
            operator JsonHashTable();

        private:
            JsonValue()
                : json(0), token(0)
            {

            }

            JsonValue(char* json, jsmntok_t* token)
                : json(json), token(token)
            {

            }

            char* json;
            jsmntok_t* token;
        };
    }
}