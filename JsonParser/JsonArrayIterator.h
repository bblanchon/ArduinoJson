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
        // An iterator for JsonArray
        class JsonArrayIterator : JsonToken
        {
        public:

            // Create an iterator pointing at the specified JsonToken
            JsonArrayIterator(JsonToken token)
                : JsonToken(token)
            {

            }

            // Move iterator forward
            void operator++()
            {
                *this = JsonArrayIterator(nextSibling());
            }

            // Get the value pointed by the iterator
            JsonValue operator*() const
            {
                return JsonValue(*this);
            }

            // Test iterator equality
            bool operator!= (const JsonArrayIterator& other) const
            {
                return JsonToken::operator!=(other);
            }
        };
    }
}
