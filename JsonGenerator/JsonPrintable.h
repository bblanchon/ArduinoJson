/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonValue.h"
#include "Print.h"
#include "Printable.h"
#include "JsonPrettyPrint.h"

namespace ArduinoJson
{
    namespace Generator
    {
        class JsonPrintable : public Printable
        {
        public:

            size_t printTo(char* buffer, size_t bufferSize)
            {
                using namespace Internals;

                StringBuilder sb(buffer, bufferSize);
                return printTo(sb);
            }

            size_t prettyPrintTo(IndentedPrint& p) const
            {
                JsonPrettyPrint decorator(p);
                return printTo(decorator);
            }

            size_t prettyPrintTo(Print& p) const
            {
                IndentedPrint decorator(p);
                return printTo(decorator);
            }

            virtual size_t printTo(Print& p) const = 0;
        };
    }
}