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
        class JsonObjectIterator : public JsonToken
        {
        public:

            JsonObjectIterator(JsonToken token)
                : JsonToken(token)
            {
            }

            void operator++()
            {
                *this = JsonObjectIterator(nextSibling().nextSibling());
            }

            JsonPair operator*() const
            {
                return JsonPair(*this);
            }
        };
    }
}