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
                _printToImpl = &printBoolTo;
                _content.asBool = value;
            }

            void operator=(long value)
            {
                _printToImpl = &printLongTo;
                _content.asLong = value;
            }

            void operator=(int value)
            {
                _printToImpl = &printLongTo;
                _content.asLong = value;
            }

            void operator=(const Printable& value)
            {
                _printToImpl = &printPrintableTo;
                _content.asPrintable = &value;
            }

            void operator=(const char* value)
            {
                _printToImpl = &printStringTo;
                _content.asString = value;
            }

            void operator=(double value)
            {
                set<2>(value);
            }

            template <int DIGITS>
            void set(double value)
            {
                _printToImpl = &printDoubleTo < DIGITS > ;
                _content.asDouble = value;
            }

            operator bool()
            {
                return _content.asBool;
            }

            operator const char*()
            {
                return _content.asString;
            }

            operator double()
            {
                return _content.asDouble;
            }

            operator float()
            {
                return static_cast<float>(_content.asDouble);
            }

            operator int()
            {
                return _content.asLong;
            }

            operator long()
            {
                return _content.asLong;
            }

            operator const Printable&()
            {
                return *_content.asPrintable;
            }

            size_t printTo(Print& p) const
            {
                // handmade polymorphism
                return _printToImpl(_content, p);
            }

            void reset()
            {
                _content.asDouble = 0;
                _printToImpl = printStringTo;
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

            Content _content;

            size_t(*_printToImpl)(const Content&, Print&);

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
