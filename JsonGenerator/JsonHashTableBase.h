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
                if (itemCount >= capacity) return;

                items[itemCount].key.set(key);
                items[itemCount].value.set(value);
                itemCount++;
            }

            template<int DIGITS>
            void add(const char* key, double value)
            {
                if (itemCount >= capacity) return;

                items[itemCount].key.set(key);
                items[itemCount].value.set<DIGITS>(value);
                itemCount++;
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
                : items(items), capacity(capacity), itemCount(0)
            {
            }

        private:
            KeyValuePair* items;
            int itemCount;
            int capacity;
        };
    }
}