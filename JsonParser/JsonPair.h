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
        class JsonPair : public JsonToken
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
                return JsonValue(nextSibling());
            }
        };
    }
}
