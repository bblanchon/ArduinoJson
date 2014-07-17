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

        class JsonHashTable
        {
            friend class JsonValue;

        public:

            JsonHashTable()	{}

            bool success()
            {
                return value.success();
            }
            
            JsonValue operator[](const char* key)
            {
                return value[key];
            }

            bool containsKey(const char* key)
            {
                return value[key];
            }

            DEPRECATED JsonArray getArray(const char* key);

            DEPRECATED bool getBool(const char* key)
            {
                return value[key];
            }

            DEPRECATED double getDouble(const char* key)
            {
                return value[key];
            }

            DEPRECATED JsonHashTable getHashTable(const char* key)
            {
                return value[key];
            }

            DEPRECATED long getLong(const char* key)
            {
                return value[key];
            }

            DEPRECATED char* getString(const char* key)
            {
                return value[key];
            }

        private:

            JsonHashTable(JsonValue& value)
                : value(value)
            {

            }

            JsonValue value;
        };
    }
}