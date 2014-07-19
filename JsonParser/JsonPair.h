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
        class JsonPair : JsonToken
        {
        public:
            JsonPair(JsonToken token)
                : JsonToken(token)
            {

            }

            const char* key()
            {
                return getText();
            }

            JsonValue value()
            {
                return nextSibling();
            }
        };
    }
}
