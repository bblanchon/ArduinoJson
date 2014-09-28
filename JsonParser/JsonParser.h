/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonParserBase.h"

namespace ArduinoJson
{
    namespace Parser
    {
        // The JSON parser.
        //
        // You need to specifiy the number of token to be allocated for that parser.
        //
        // CAUTION: JsonArray, JsonObject and JsonValue contain pointers to tokens of the
        // JsonParser, so they need the JsonParser to be in memory to work.
        // As a result, you must not create JsonArray, JsonObject or JsonValue that have a
        // longer life that the JsonParser.
        template <int MAX_TOKENS>
        class JsonParser : public JsonParserBase
        {
        public:
            JsonParser()
                : JsonParserBase(_tokens, MAX_TOKENS)
            {
            }

        private:
            jsmntok_t _tokens[MAX_TOKENS];
        };
    }
}
