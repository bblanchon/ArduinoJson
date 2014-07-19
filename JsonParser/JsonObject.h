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

        class JsonObject : JsonValue
        {
        public:
            JsonObject()
            {

            }

            JsonObject(JsonValue value)
                : JsonValue(value)
            {

            }

            bool success()
            {
                return isObject();
            }
            
            JsonValue operator[](const char* key);

            bool containsKey(const char* key)
            {
                return operator[](key).success();
            }

            JsonObjectIterator begin()
            {
                return isObject() ? firstChild() : null();
            }

            JsonObjectIterator end()
            {
                return isObject() ? nextSibling() : null();
            }

            DEPRECATED JsonArray getArray(const char* key);

            DEPRECATED bool getBool(const char* key)
            {
                return operator[](key);
            }

            DEPRECATED double getDouble(const char* key)
            {
                return operator[](key);
            }

            DEPRECATED JsonObject getHashTable(const char* key)
            {
                return operator[](key);
            }

            DEPRECATED long getLong(const char* key)
            {
                return operator[](key);
            }

            DEPRECATED char* getString(const char* key)
            {
                return operator[](key);
            }

            static JsonObject null()
            {
                return JsonObject(JsonToken::null());
            }
        };

        typedef JsonObject JsonHashTable;
    }
}