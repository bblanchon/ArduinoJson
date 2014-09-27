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

        // A JSON Object (ie hash-table/dictionary)
        class JsonObject : JsonValue
        {
        public:

            // Create an invalid JsonObject
            JsonObject()
            {

            }

            // Convert a JsonValue into a JsonObject
            JsonObject(JsonValue value)
                : JsonValue(value)
            {

            }

            // Tell if the object is valid
            bool success()
            {
                return isObject();
            }           

            // Get the value associated with the specified key.
            JsonValue operator[](const char* key)
            {
                return JsonValue::operator[](key);
            }

            // Tell if the specified key exists in the object.
            bool containsKey(const char* key)
            {
                return operator[](key).success();
            }

            // Get an iterator pointing at the beginning of the object
            JsonObjectIterator begin()
            {
                return isObject() ? firstChild() : null();
            }

            // Get an iterator pointing at the end of the object
            JsonObjectIterator end()
            {
                return isObject() ? nextSibling() : null();
            }

            // Obsolete: Use operator[] instead
            DEPRECATED JsonArray getArray(const char* key);

            // Obsolete: Use operator[] instead
            DEPRECATED bool getBool(const char* key)
            {
                return operator[](key);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED double getDouble(const char* key)
            {
                return operator[](key);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED JsonObject getHashTable(const char* key)
            {
                return operator[](key);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED long getLong(const char* key)
            {
                return operator[](key);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED char* getString(const char* key)
            {
                return operator[](key);
            }
        };

        // Obsolete: Use JsonObject instead
        DEPRECATED typedef JsonObject JsonHashTable;
    }
}
