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
        class JsonArray;

        class JsonHashTable : public JsonObjectBase
        {
            friend class JsonParserBase;
            friend class JsonValue;

        public:

            JsonHashTable()	{}

            bool success()
            {
                return JsonObjectBase::success() && tokens->type == JSMN_OBJECT;
            }
            
            JsonValue operator[](const char* key);

            bool containsKey(const char* key)
            {
                return (*this)[key].success();
            }

            JsonArray getArray(const char* key) DEPRECATED;

            bool getBool(const char* key) DEPRECATED
            {
                return (bool) (*this)[key];
            }

            double getDouble(const char* key) DEPRECATED
            {
                return (double) (*this)[key];
            }

            JsonHashTable getHashTable(const char* key) DEPRECATED
            {
                return (JsonHashTable) (*this)[key];
            }

            long getLong(const char* key) DEPRECATED
            {
                return (long) (*this)[key];
            }

            char* getString(const char* key) DEPRECATED
            {
                return (char*) (*this)[key];
            }

        private:

            JsonHashTable(char* json, jsmntok_t* tokens)
                : JsonObjectBase(json, tokens)
            {

            }
        };
    }
}