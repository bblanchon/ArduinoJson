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
        // A JSON key-value pair, as a part of a JSON object
        class JsonPair : JsonToken
        {
        public:
            // Convert a JsonToken to a JsonPair
            JsonPair(JsonToken token)
                : JsonToken(token)
            {
            }

            // Get the key
            const char* key()
            {
                return getText();
            }

            // Get the value
            JsonValue value()
            {
                return nextSibling();
            }
        };
    }
}