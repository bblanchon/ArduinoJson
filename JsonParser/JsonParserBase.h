/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonArray.h"
#include "JsonObject.h"

namespace ArduinoJson
{
    namespace Parser
    {
        // Base class for the JSON parser, in case you want to provide your own buffer
        class JsonParserBase
        {
        public:

            // Create a JSON parser using the provided buffer
            JsonParserBase(jsmntok_t* tokens, int maxTokens)
                : _tokens(tokens), _maxTokens(maxTokens)
            {
            }

            // Parse the JSON string and return a array
            //
            // The content of the string may be altered to add '\0' at the
            // end of string tokens            
            JsonValue parse(char* json);

            // Obsolete: use parse() instead
            DEPRECATED JsonArray parseArray(char* json)
            {
                return parse(json);
            }

            // Obsolete: use parse() instead
            DEPRECATED JsonObject parseHashTable(char* json)
            {
                return parse(json);
            }

        private:
            jsmntok_t* _tokens;
            int _maxTokens;
        };
    }
}
