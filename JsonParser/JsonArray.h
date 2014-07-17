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
        class JsonHashTable;
               
        class JsonArray
        {
        public:

            JsonArray()	{}

            JsonArray(JsonValue& value)
                : value(value)
            {

            }
            
            bool success()
            {
                return value.success();
            }

            int size()
            {
                return value.size();
            }

            JsonValue operator[](int index)
            {
                return value[index];
            }

            JsonArrayIterator begin()
            {
                return JsonArrayIterator(value);
            }

            JsonArrayIterator end()
            {
                return JsonArrayIterator();
            }

            DEPRECATED int getLength()
            {
                return size();
            }
                      
            DEPRECATED JsonArray getArray(int index)
            {
                return (JsonArray) (*this)[index];
            }

            DEPRECATED bool getBool(int index)
            {
                return (bool) (*this)[index];
            }

            DEPRECATED double getDouble(int index)
            {
                return (double) (*this)[index];
            }

            DEPRECATED JsonHashTable getHashTable(int index);

            DEPRECATED long getLong(int index)
            {
                return (long) (*this)[index];
            }

            DEPRECATED char* getString(int index)
            {
                return (char*) (*this)[index];
            }

        private:

            JsonValue value;
        };
    }
}