/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "EscapedString.h"
#include "Printable.h"
#include "StringBuilder.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonValue
        {
        public:

            void operator=(bool value)
            {
                printToImpl = &printBoolTo;
                content.asBool = value;
            }

            void operator=(long value)
            {
                printToImpl = &printLongTo;
                content.asLong = value;
            }

            void operator=(int value)
            {
                printToImpl = &printLongTo;
                content.asLong = value;
            }

            void operator=(const Printable& value)
            {
                printToImpl = &printPrintableTo;
                content.asPrintable = &value;
            }

            void operator=(const char* value)
            {
                printToImpl = &printStringTo;
                content.asString = value;
            }

            void operator=(double value)
            {
                set<2>(value);
            }

            template <int DIGITS>
            void set(double value)
            {
                printToImpl = &printDoubleTo < DIGITS > ;
                content.asDouble = value;
            }

            operator bool()
            {
                return content.asBool;
            }

            operator const char*()
            {
                return content.asString;
            }

            operator double()
            {
                return content.asDouble;
            }

            operator float()
            {
                return (float)content.asDouble;
            }

            operator int()
            {
                return content.asLong;
            }

            operator long()
            {
                return content.asLong;
            }

            operator const Printable&()
            {
                return *content.asPrintable;
            }

            size_t printTo(Print& p) const
            {
                // handmade polymorphism
                return printToImpl(content, p);
            }

            void reset()
            {
                content.asDouble = 0;
                printToImpl = printStringTo;
            }

        private:
            union Content
            {
                bool                asBool;
                double              asDouble;
                long                asLong;
                const Printable*    asPrintable;
                const char*         asString;
            };

            Content content;

            size_t(*printToImpl)(const Content&, Print&);

            static size_t printBoolTo(const Content&, Print&);
            static size_t printLongTo(const Content&, Print&);
            static size_t printPrintableTo(const Content&, Print&);
            static size_t printStringTo(const Content&, Print&);

            template <int DIGITS>
            static size_t printDoubleTo(const Content& c, Print& p)
            {
                return p.print(c.asDouble, DIGITS);
            }
        };
    }
}