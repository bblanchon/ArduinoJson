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
            JsonArrayBase(JsonValue* items, int capacity)
                : items(items), capacity(capacity), count(0)
            {

            }

            void add(const Printable& nestedObject)
            {
                if (count < capacity)
                    items[count++] = nestedObject;
            }

            template<typename T>
            void add(T value)
            {
                if (count < capacity)
                    items[count++] = value;
            }

            template<int DIGITS>
            void add(double value)
            {
                if (count >= capacity) return;

                JsonValue& v = items[count++];
                v.set<DIGITS>(value);
            }

            virtual size_t printTo(Print& p) const;

            using JsonPrintable::printTo;

        private:
            JsonValue* items;
            int capacity, count;
        };
    }
}