/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"
#include "JsonValue.h"

#define DEPRECATED

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonHashTable;

        class JsonArray : public JsonObjectBase
        {
            friend class JsonParserBase;
            friend class JsonValue;

        public:

            JsonArray()	{}

            int getLength()
            {
                return tokens != 0 ? tokens[0].size : 0;
            }

            JsonValue operator[](int index);
                      
            JsonArray getArray(int index) DEPRECATED
            {
                return (JsonArray) (*this)[index];
            }

            bool getBool(int index) DEPRECATED
            {
                return (bool) (*this)[index];
            }

            double getDouble(int index) DEPRECATED
            {
                return (double) (*this)[index];
            }

            JsonHashTable getHashTable(int index) DEPRECATED;

            long getLong(int index) DEPRECATED
            {
                return (long) (*this)[index];
            }

            char* getString(int index) DEPRECATED
            {
                return (char*) (*this)[index];
            }

        private:

            JsonArray(char* json, jsmntok_t* tokens);
        };
    }
}