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
        /*
        * The JSON parser.
        *
        * You need to specifiy the number of token to be allocated for that parser.
        * Values from 16 to 32 are recommended.
        * The parser size will be MAX_TOKEN*8 bytes.
        * Don't forget that the memory size of standard Arduino board is only 2KB
        *
        * CAUTION: JsonArray and JsonHashTable contain pointers to tokens of the
        * JsonParser, so they need the JsonParser to be in memory to work.
        * As a result, you must not create JsonArray and JsonHashTable that have a
        * longer life that the JsonParser.
        */
        template <int MAX_TOKENS>
        class JsonParser : public JsonParserBase
        {
        public:
            JsonParser()
                : JsonParserBase(tokens, MAX_TOKENS)
            {
            }

        private:
            jsmntok_t tokens[MAX_TOKENS];
        };
    }
}