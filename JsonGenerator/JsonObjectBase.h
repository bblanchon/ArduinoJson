/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonPrintable.h"
#include "EscapedString.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonObjectBase : public JsonPrintable
        {
        public:

            template<typename T>
            void add(const char* key, T value)
            {
                KeyValuePair* pair = getMatchingPair(key);
                if (!pair) return;

                pair->value.set(value);
            }

            template<int DIGITS>
            void add(const char* key, double value)
            {
                KeyValuePair* pair = getMatchingPair(key);
                if (!pair) return;

                pair->value.set<DIGITS>(value);
            }           

            using JsonPrintable::printTo;

            virtual size_t printTo(Print& p) const;

        protected:

            struct KeyValuePair
            {
                Internals::EscapedString  key;
                Internals::JsonValue      value;
            };

            JsonObjectBase(KeyValuePair* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {
            }

            KeyValuePair* getMatchingPair(const char* key);

        private:
            KeyValuePair* items;
            int capacity, count;
        };
    }
}