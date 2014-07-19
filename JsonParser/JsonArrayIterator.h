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
        class JsonArrayIterator : JsonToken
        {
        public:

            JsonArrayIterator(JsonToken token)
                : JsonToken(token)
            {

            }

            void operator++()
            {
                *this = JsonArrayIterator(nextSibling());
            }

            JsonValue operator*() const
            {
                return JsonValue(*this);
            }

            bool operator!= (const JsonArrayIterator& other) const
            {
                return JsonToken::operator!=(other);
            }
        };
    }
}