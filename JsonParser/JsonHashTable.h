/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonValue.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonArray;

        class JsonHashTable
        {
            friend class JsonValue;

        public:

            JsonHashTable()	
                : token(Internal::JsonToken::null())
            {
            }

            bool success()
            {
                return token.isObject();
            }
            
            JsonValue operator[](const char* key)
            {
                return getValue(key);
            }

            bool containsKey(const char* key)
            {
                return getValue(key).success();
            }

            DEPRECATED JsonArray getArray(const char* key);

            DEPRECATED bool getBool(const char* key)
            {
                return getValue(key);
            }

            DEPRECATED double getDouble(const char* key)
            {
                return getValue(key);
            }

            DEPRECATED JsonHashTable getHashTable(const char* key)
            {
                return getValue(key);
            }

            DEPRECATED long getLong(const char* key)
            {
                return getValue(key);
            }

            DEPRECATED char* getString(const char* key)
            {
                return getValue(key);
            }

            static JsonHashTable null()
            {
                return JsonHashTable();
            }

        private:

            JsonHashTable(char* json, Internal::JsonToken token)
                : json(json), token(token)
            {

            }

            char* json;
            Internal::JsonToken token;

            JsonValue getValue(const char* key);
        };
    }
}