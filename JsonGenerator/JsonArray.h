/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonArrayBase.h"

namespace ArduinoJson
{
    namespace Generator
    {
        template<int N>
        class JsonArray : public JsonArrayBase
        {
        public:
            JsonArray()
                : JsonArrayBase(items, N)
            {

            }         

        private:
            JsonValue items[N];
        };
    }
}