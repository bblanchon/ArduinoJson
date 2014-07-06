/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "Printable.h"
#include "StringBuilder.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonValue
        {
        public:

            JsonValue()
            {
            }

            JsonValue(bool value)
                : printToImpl(&printBoolTo)
            {
                content.asBool = value;
            }

            JsonValue(double value, uint8_t digits = 2)
                : printToImpl(&printDoubleTo)
            {
                content.asDouble.value = value;
                content.asDouble.digits = digits;
            }

            JsonValue(long value)
                : printToImpl(&printLongTo)
            {
                content.asLong = value;
            }

            JsonValue(int value)
                : printToImpl(&printLongTo)
            {
                content.asLong = value;
            }

            JsonValue(Printable& value)
                : printToImpl(&printPrintableTo)
            {
                content.asPrintable = &value;
            }

            JsonValue(const char* value)
                : printToImpl(&printStringTo)
            {
                content.asString = value;
            }

            size_t printTo(Print& p) const
            {
                // handmade polymorphism
                return printToImpl(content, p);
            }

        private:
            union Content
            {
                bool        asBool;
                long        asLong;
                Printable*  asPrintable;
                const char* asString;

                struct {
                    double value;
                    uint8_t digits;
                } asDouble;
            };

            Content content;

            size_t(*printToImpl)(const Content&, Print& p);

            static size_t printBoolTo(const Content&, Print& p);
            static size_t printDoubleTo(const Content&, Print& p);
            static size_t printLongTo(const Content&, Print& p);
            static size_t printPrintableTo(const Content&, Print& p);
            static size_t printStringTo(const Content&, Print& p);
        };
    }
}