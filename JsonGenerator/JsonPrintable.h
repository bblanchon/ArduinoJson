/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "JsonValue.h"
#include "Print.h"
#include "Printable.h"

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

            virtual size_t printTo(Print& p) const = 0;
        };
    }
}