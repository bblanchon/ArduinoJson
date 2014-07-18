/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonValue.h"
#include "JsonToken.h"

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
                JsonArrayIterator prev = *this;
                token = token.nextSibling();
                return prev;
            }

            JsonValue operator*()
            {
                return JsonValue(json, token);
            }

            bool operator !=(const JsonArrayIterator& other)
            {
                return token != other.token || json != other.json;
            }

        private:

            char* json;
            Internal::JsonToken token;

            JsonArrayIterator()
                : json(0), token(0)
            {

            }

            JsonArrayIterator(char* json, Internal::JsonToken& token)
                : json(json), token(token)
            {

            }
        };
    }
}