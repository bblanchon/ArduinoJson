/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonValue.h"
#include "JsonPair.h"
#include "JsonToken.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonObjectIterator
        {
        public:

            JsonObjectIterator(char* json, Internal::JsonToken token)
                : json(json), token(token)
            {

            }

            void operator++()
            {
                token = token.nextSibling().nextSibling();
            }

            JsonPair operator*() const
            {
                return JsonPair(json, token);
            }

            bool operator !=(const JsonObjectIterator& other)
            {
                return token != other.token;
            }

        private:

            char* json;
            Internal::JsonToken token;
        };
    }
}