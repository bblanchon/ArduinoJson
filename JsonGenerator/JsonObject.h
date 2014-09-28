/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"

#ifndef ARDUINO_JSON_NO_DEPRECATION_WARNING
#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#endif
#else
#define DEPRECATED
#endif

namespace ArduinoJson
{
    namespace Generator
    {
        template <int N>
        class JsonObject : public JsonObjectBase
        {
        public:
            JsonObject()
                : JsonObjectBase(_items, N)
            {
            }

        private:
            KeyValuePair _items[N];
        };

        
        // Obsolete: use JsonObject instead
        template <int N>
        class DEPRECATED JsonHashTable : public JsonObject<N>
        {            
        };
    }
}
