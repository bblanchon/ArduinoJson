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

            JsonValue& operator[](const char*);

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
                Internals::EscapedString  key;
                JsonValue      value;
            };

            JsonObjectBase(KeyValuePair* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {
            }

        private:
            KeyValuePair* items;
            int capacity, count;
        };
    }
}