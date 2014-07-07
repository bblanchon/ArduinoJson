/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonObjectBase.h"
#include "StringBuilder.h"

namespace ArduinoJson
{
    namespace Generator
    {
        template<int N>
        class JsonArray : public JsonObjectBase
        {
        public:
            JsonArray()
            {
                itemCount = 0;
            }

            template<typename T>
            void add(T value)
            {
                if (itemCount >= N) return;

                items[itemCount++].set(value);
            }

            void add(double value)
            {
                add<2>(value);
            }

            template<int DIGITS>
            void add(double value)
            {
                if (itemCount >= N) return;

                Internals::JsonValue& v = items[itemCount++];
                v.set<DIGITS>(value);
            }

            using JsonObjectBase::printTo;

        private:
            Internals::JsonValue items[N];
            int itemCount;

            virtual size_t printTo(Print& p) const
            {
                size_t n = 0;

                n += p.write('[');

                for (int i = 0; i < itemCount; i++)
                {
                    if (i > 0)
                    {
                        n += p.write(',');
                    }

                    n += items[i].printTo(p);
                }

                n += p.write(']');

                return n;
            }
        };
    }
}