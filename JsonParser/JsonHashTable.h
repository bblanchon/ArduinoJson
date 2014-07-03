/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonObjectBase.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonArray;

        class JsonHashTable : public JsonObjectBase
        {
            template <int N>
            friend class JsonParser;

            friend class JsonArray;

        public:

            JsonHashTable() {}

            bool containsKey(const char* key);

            JsonArray getArray(const char* key);
            bool getBool(const char* key);
            double getDouble(const char* key);
            JsonHashTable getHashTable(const char* key);
            long getLong(const char* key);
            char* getString(const char* key);

        private:

            JsonHashTable(char* json, jsmntok_t* tokens);
            jsmntok_t* getToken(const char* key);
        };
    }
}