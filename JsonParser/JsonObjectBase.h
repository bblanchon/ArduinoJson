/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "jsmn.h"

namespace ArduinoJson
{
    namespace Parser
    {
        class JsonObjectBase
        {
        public:

            JsonObjectBase()
                : json(0), tokens(0)
            {

            }

            bool success()
            {
                return json != 0 && tokens != 0;
            }

        protected:

            JsonObjectBase(char* json, jsmntok_t* tokens)
                : json(json), tokens(tokens)
            {

            }

            static int getNestedTokenCount(jsmntok_t* token);

            char* json;
            jsmntok_t* tokens;
        };
    }
}