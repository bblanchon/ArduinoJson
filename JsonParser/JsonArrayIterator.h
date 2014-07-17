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
        class JsonHashTable;

        class JsonArrayIterator
        {
            friend class JsonArray;

        public:

            JsonArrayIterator operator++()
            {
                tokens++;
                return *this;                    
            }

            JsonValue operator*()
            {
                return JsonValue(json, tokens);
            }

            bool operator !=(const JsonArrayIterator& other)
            {
                return tokens != other.tokens || json != other.json;
            }

        private:

            char* json;
            jsmntok_t* tokens;

            JsonArrayIterator()
                : json(0), tokens(0)
            {

            }

            JsonArrayIterator(JsonValue& value)
                : json(value.json), tokens(value.tokens)
            {

            }
        };
    }
}