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
        // An iterator for JsonObject
        class JsonObjectIterator : JsonToken
        {
        public:

            // Create an iterator pointing at the specified token
            JsonObjectIterator(JsonToken token)
                : JsonToken(token)
            {
            }

            // Move to the next JsonPair
            void operator++()
            {
                *this = JsonObjectIterator(nextSibling().nextSibling());
            }

            // Get the JsonPair pointed by the iterator
            JsonPair operator*() const
            {
                return JsonPair(*this);
            }

            // Test iterator equality
            bool operator!= (const JsonObjectIterator& other) const
            {
                return JsonToken::operator!=(other);
            }

            // Get the key of the JsonPair pointed by the iterator
            const char* key() const
            {
                return operator*().key();
            }

            // Get the key of the JsonPair pointed by the iterator
            JsonValue value() const
            {
                return operator*().value();
            }
        };
    }
}
