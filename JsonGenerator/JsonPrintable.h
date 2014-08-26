/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "Print.h"
#include "Printable.h"
#include "IndentedPrint.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonPrintable : public Printable
        {
        public:

            virtual size_t printTo(Print& p) const = 0;

            size_t printTo(char* buffer, size_t bufferSize);

            size_t prettyPrintTo(IndentedPrint& p) const;
            size_t prettyPrintTo(Print& p) const;
        };
    }
}