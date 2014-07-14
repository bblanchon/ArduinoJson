/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonHashTable.h"
#include "JsonArray.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonParserBase
        {
        public:

            JsonParserBase(jsmntok_t* tokens, int maxTokens)
                : tokens(tokens), maxTokens(maxTokens)
            {
            }

            /*
            * Parse the JSON string and return a array.
            *
            * The content of the string may be altered to add '\0' at the
            * end of string tokens
            */
            JsonArray parseArray(char* json)
            {
                return JsonArray(json, parse(json));
            }

            /*
            * Parse the JSON string and return a array.
            *
            * The content of the string may be altered to add '\0' at the
            * end of string tokens
            */
            JsonHashTable parseHashTable(char* json)
            {
                return JsonHashTable(json, parse(json));
            }

        private:
            jsmntok_t* tokens;
            int maxTokens;

            jsmntok_t* parse(char* json);
        };
    }
}