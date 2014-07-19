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
               
        class JsonArray : public JsonToken
        {          
        public:

            JsonArray() 
            {            
            }

            JsonArray(JsonToken token) 
                : JsonToken(token) 
            {
            }

            bool success()
            {
                return isArray();
            }

            int size()
            {
                return isArray() ? JsonToken::size() : 0;
            }

            JsonValue operator[](int index)
            {
                return getValue(index);
            }

            JsonArrayIterator begin()
            {
                return firstChild();
            }

            JsonArrayIterator end()
            {
                return nextSibling();
            }

            DEPRECATED int getLength()
            {
                return size();
            }
                      
            DEPRECATED JsonArray getArray(int index)
            {
                return getValue(index);
            }

            DEPRECATED bool getBool(int index)
            {
                return getValue(index);
            }

            DEPRECATED double getDouble(int index)
            {
                return getValue(index);
            }

            DEPRECATED JsonObject getHashTable(int index);

            DEPRECATED long getLong(int index)
            {
                return getValue(index);
            }

            DEPRECATED char* getString(int index)
            {
                return getValue(index);
            }

        private:

            JsonValue getValue(int index);
        };
    }
}