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

            JsonValue parse(char* json);

            /*
            * Parse the JSON string and return a array.
            *
            * The content of the string may be altered to add '\0' at the
            * end of string tokens
            */
            DEPRECATED JsonArray parseArray(char* json)
            {
                return (JsonArray)parse(json);
            }

            /*
            * Parse the JSON string and return a array.
            *
            * The content of the string may be altered to add '\0' at the
            * end of string tokens
            */
            DEPRECATED JsonHashTable parseHashTable(char* json)
            {
                return (JsonHashTable)parse(json);
            }

        private:
            jsmntok_t* tokens;
            int maxTokens;
        };
    }
}