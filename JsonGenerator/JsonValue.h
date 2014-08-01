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

            void operator=(Printable& value)
            {
                printToImpl = &printPrintableTo;
                content.asPrintable = &value;
            }

            void operator=(const char* value)
            {
                printToImpl = &printStringTo;
                content.asString.set(value);
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
                return "";
            }

            operator double()
            {
                return content.asDouble;
            }

            operator int()
            {
                return content.asLong;
            }

            operator long()
            {
                return content.asLong;
            }

            size_t printTo(Print& p) const
            {
                // handmade polymorphism
                return printToImpl(content, p);
            }

            static JsonValue& null()
            {
                return nullInstance;
            }

        private:
            union Content
            {
                bool asBool;
                long asLong;
                Printable* asPrintable;
                Internals::EscapedString asString;
                double asDouble;
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

            static JsonValue nullInstance;
        };
    }
}