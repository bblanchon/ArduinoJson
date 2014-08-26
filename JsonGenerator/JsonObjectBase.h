/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonPrintable.h"
#include "JsonValue.h"
#include "EscapedString.h"

namespace ArduinoJson
{
    namespace Generator
    {
        typedef const char* JsonKey;

        class JsonObjectBase : public JsonPrintable
        {
        public:
            JsonValue& operator[](JsonKey);
            bool containsKey(JsonKey) const;
            void remove(JsonKey key);

            template<typename T>
            void add(JsonKey key, T value)
            {
                operator[](key) = value;
            }

            template<int DIGITS>
            void add(JsonKey key, double value)
            {
                operator[](key).set<DIGITS>(value);
            }

            using JsonPrintable::printTo;

            virtual size_t printTo(Print& p) const;

        protected:

            struct KeyValuePair
            {
                JsonKey     key;
                JsonValue   value;
            };

            JsonObjectBase(KeyValuePair* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {
            }

        private:
            KeyValuePair* items;
            int capacity, count;
            static JsonValue nullValue;

            KeyValuePair* getMatchingPair(JsonKey key) const;
        };
    }
}