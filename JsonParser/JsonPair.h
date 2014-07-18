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
        class JsonPair
        {
        public:

            JsonPair(char* json, Internal::JsonToken token)
                : json(json), token(token)
            {

            }

            const char* key()
            {
                return token.getText(json);
            }

            JsonValue value()
            {
                return JsonValue(json, token.nextSibling());
            }

        private:
            char* json;
            Internal::JsonToken token;
        };
    }
}
