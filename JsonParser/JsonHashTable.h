/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonObjectBase.h"
#include "JsonValue.h"

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

            DEPRECATED JsonArray getArray(const char* key);

            DEPRECATED bool getBool(const char* key)
            {
                return (bool) (*this)[key];
            }

            DEPRECATED double getDouble(const char* key)
            {
                return (double) (*this)[key];
            }

            DEPRECATED JsonHashTable getHashTable(const char* key)
            {
                return (JsonHashTable) (*this)[key];
            }

            DEPRECATED long getLong(const char* key)
            {
                return (long) (*this)[key];
            }

            DEPRECATED char* getString(const char* key)
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