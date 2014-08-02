/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonPrintable.h"
#include "EscapedString.h"
#include <string.h> // for strcmp

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonObjectBase : public JsonPrintable
        {
        public:

            JsonValue& operator[](const char*);

            bool containsKey(const char*) const;

            template<typename T>
            void add(const char* key, T value)
            {
                operator[](key) = value;
            }

            template<int DIGITS>
            void add(const char* key, double value)
            {
                operator[](key).set<DIGITS>(value);
            }

            using JsonPrintable::printTo;

            virtual size_t printTo(Print& p) const;

        protected:

            struct KeyValuePair
            {
                const char* key;
                JsonValue   value;

                bool matches(const char* candidateKey) const
                {
                    return strcmp(key, candidateKey) == 0;
                }
            };

            JsonObjectBase(KeyValuePair* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {
            }

        private:
            KeyValuePair* items;
            int capacity, count;

            static JsonValue nullValue;
        };
    }
}