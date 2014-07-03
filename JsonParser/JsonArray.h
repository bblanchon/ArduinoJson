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
        class JsonHashTable;

        class JsonArray : public JsonObjectBase
        {
            template <int N>
            friend class JsonParser;

            friend class JsonHashTable;

        public:

            JsonArray()	{}

            int getLength()
            {
                return tokens != 0 ? tokens[0].size : 0;
            }

            JsonArray getArray(int index);
            bool getBool(int index);
            double getDouble(int index);
            JsonHashTable getHashTable(int index);
            long getLong(int index);
            char* getString(int index);

        private:

            JsonArray(char* json, jsmntok_t* tokens);
            jsmntok_t* getToken(int index);
        };
    }
}