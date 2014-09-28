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
                : _items(items), _capacity(capacity), _count(0)
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
                if (_count >= _capacity) return;

                JsonValue& v = _items[_count++];
                v.set<DIGITS>(value);
            }

            virtual size_t printTo(Print& p) const;

            using JsonPrintable::printTo;

        private:
            JsonValue* _items;
            int _capacity, _count;

            template<typename T>
            void addIfPossible(T value)
            {
                if (_count < _capacity)
                    _items[_count++] = value;
            }
        };
    }
}
