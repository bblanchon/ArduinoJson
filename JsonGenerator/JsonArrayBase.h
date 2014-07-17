/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonPrintable.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonArrayBase : public JsonPrintable
        {
        public:
            JsonArrayBase(Internals::JsonValue* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {

            }

            template<typename T>
            void add(T value)
            {
                if (count >= capacity) return;

                items[count++].set(value);
            }

            template<int DIGITS>
            void add(double value)
            {
                if (count >= capacity) return;

                Internals::JsonValue& v = items[count++];
                v.set<DIGITS>(value);
            }

            virtual size_t printTo(Print& p) const;

            using JsonPrintable::printTo;

        private:
            Internals::JsonValue* items;
            int capacity, count;
        };
    }
}