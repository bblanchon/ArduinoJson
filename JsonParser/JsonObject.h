/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonValue.h"
#include "JsonObjectIterator.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonArray;

        class JsonObject : public JsonToken
        {
        public:
            JsonObject()
            {

            }

            JsonObject(JsonToken token)
                : JsonToken(token)
            {

            }

            bool success()
            {
                return isObject();
            }
            
            JsonValue operator[](const char* key)
            {
                return getValue(key);
            }

            bool containsKey(const char* key)
            {
                return getValue(key).success();
            }

            JsonObjectIterator begin()
            {
                return firstChild();
            }

            JsonObjectIterator end()
            {
                return nextSibling();
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

            DEPRECATED JsonObject getHashTable(const char* key)
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

            static JsonObject null()
            {
                return JsonObject(JsonToken::null());
            }

        private:

            JsonValue getValue(const char* key);
        };

        typedef JsonObject JsonHashTable;
    }
}