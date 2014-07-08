/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonHashTableBase.h"

namespace ArduinoJson
{
    namespace Generator
    {
        template<int N>
        class JsonHashTable : public JsonHashTableBase
        {
        public:
            JsonHashTable()
                : JsonHashTableBase(items, N)
            {
            }            

        private:
            KeyValuePair items[N];     
        };
    }
}