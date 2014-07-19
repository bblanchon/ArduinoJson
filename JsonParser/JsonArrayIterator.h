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
        class JsonArrayIterator : public JsonToken
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
        };
    }
}