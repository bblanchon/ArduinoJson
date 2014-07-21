/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonValue.h"
#include "JsonArrayIterator.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonObject;
                
        // A JSON array
        class JsonArray : JsonValue
        {          
        public:

            // Create an invalid array
            JsonArray() 
            {            
            }

            // Convert a JsonValue into a JsonArray
            JsonArray(JsonValue value)
                : JsonValue(value)
            {
            }

            // Tell if the array is valid
            bool success()
            {
                return isArray();
            }

            // Get the JsonValue at specified index
            JsonValue operator[](int index)
            {
                return JsonValue::operator[](index);
            }

            // Get the size of the array
            int size()
            {
                return isArray() ? childrenCount() : 0;
            }

            // Get an iterator pointing to the beginning of the array
            JsonArrayIterator begin()
            {
                return isArray() ? firstChild() : null();
            }

            // Gets an iterator pointing to the end of the array
            JsonArrayIterator end()
            {
                return isArray() ? nextSibling() : null();
            }

            // Obsolete: Use size() instead
            DEPRECATED int getLength()
            {
                return size();
            }
                   
            // Obsolete: Use operator[] instead
            DEPRECATED JsonArray getArray(int index)
            {
                return operator[](index);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED bool getBool(int index)
            {
                return operator[](index);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED double getDouble(int index)
            {
                return operator[](index);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED JsonObject getHashTable(int index);

            // Obsolete: Use operator[] instead
            DEPRECATED long getLong(int index)
            {
                return operator[](index);
            }

            // Obsolete: Use operator[] instead
            DEPRECATED char* getString(int index)
            {
                return operator[](index);
            }
        };
    }
}