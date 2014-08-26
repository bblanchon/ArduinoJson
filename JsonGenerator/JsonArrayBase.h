/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#pragma once

#include "JsonPrintable.h"
#include "JsonValue.h"

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

            void add(const Printable& value)
            {
                addIfPossible<const Printable&>(value);
            }

            void add(bool value)
            {
                addIfPossible<bool>(value);
            }

            void add(int value)
            {
                addIfPossible<long>(value);
            }

            void add(long value)
            {
                addIfPossible<long>(value);
            }

            void add(double value)
            {
                addIfPossible<double>(value);
            }

            void add(const char* value)
            {
                addIfPossible<const char*>(value);
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

            template<typename T>
            void addIfPossible(T value)
            {
                if (count < capacity)
                    items[count++] = value;
            }
        };
    }
}