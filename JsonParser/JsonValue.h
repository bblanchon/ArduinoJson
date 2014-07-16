/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "jsmn.h"

#include "JsonObjectBase.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonArray;
        class JsonHashTable;

        class JsonValue : public JsonObjectBase
        {
        public:
            JsonValue() {}

            JsonValue(char* json, jsmntok_t* tokens)
                : JsonObjectBase(json, tokens)
            {

            }

            operator bool();
            operator double();
            operator long();
            operator char*();
            operator JsonArray();
            operator JsonHashTable();
        };
    }
}