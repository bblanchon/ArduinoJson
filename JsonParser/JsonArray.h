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
        class JsonHashTable;
               
        class JsonArray
        {          
        public:

            JsonArray()
                : token(0)
            {

            }

            JsonArray(char* json, Internal::JsonToken token)
                : json(json), token(token)
            {

            }
            
            bool success()
            {
                return token.isArray();
            }

            int size()
            {
                return success() ? token.size() : 0;
            }

            JsonValue operator[](int index);

            JsonArrayIterator begin()
            {
                return JsonArrayIterator(json, token.firstChild());
            }

            JsonArrayIterator end()
            {
                return JsonArrayIterator(json, token.nextSibling());
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

            static JsonArray null()
            {
                return JsonArray();
            }

        private:

            char* json;
            Internal::JsonToken token;
        };
    }
}