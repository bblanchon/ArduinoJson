/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonValue.h"
#include "JsonArrayIterator.h"
#include "JsonToken.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonObject;
               
        class JsonArray : JsonValue
        {          
        public:

            JsonArray() 
            {            
            }

            JsonArray(JsonValue value)
                : JsonValue(value)
            {
            }

            bool success()
            {
                return isArray();
            }

            int size()
            {
                return isArray() ? childrenCount() : 0;
            }

            JsonValue operator[](int index);

            JsonArrayIterator begin()
            {
                return isArray() ? firstChild() : null();
            }

            JsonArrayIterator end()
            {
                return isArray() ? nextSibling() : null();
            }

            DEPRECATED int getLength()
            {
                return size();
            }
                      
            DEPRECATED JsonArray getArray(int index)
            {
                return operator[](index);
            }

            DEPRECATED bool getBool(int index)
            {
                return operator[](index);
            }

            DEPRECATED double getDouble(int index)
            {
                return operator[](index);
            }

            DEPRECATED JsonObject getHashTable(int index);

            DEPRECATED long getLong(int index)
            {
                return operator[](index);
            }

            DEPRECATED char* getString(int index)
            {
                return operator[](index);
            }
        };
    }
}