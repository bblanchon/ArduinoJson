/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonObjectBase.h"
#include "EscapedString.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonHashTableBase : public JsonObjectBase
        {
        public:

            template<typename T>
            void add(const char* key, T value)
            {
                if (count >= capacity) return;

                items[count].key.set(key);
                items[count].value.set(value);
                count++;
            }

            template<int DIGITS>
            void add(const char* key, double value)
            {
                if (count >= capacity) return;

                items[count].key.set(key);
                items[count].value.set<DIGITS>(value);
                count++;
            }

            using JsonObjectBase::printTo;

            virtual size_t printTo(Print& p) const;

        protected:

            struct KeyValuePair
            {
                Internals::EscapedString  key;
                Internals::JsonValue      value;
            };

            JsonHashTableBase(KeyValuePair* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {
            }

        private:
            KeyValuePair* items;
            int count;
            int capacity;
        };
    }
}