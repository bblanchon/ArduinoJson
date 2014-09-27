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
        // Contains methods to generate a JSON string.
        // Implemented by both JsonObject and JsonArray
        class JsonPrintable : public Printable
        {
        public:

            // Generates the compact JSON string and sends it to a Print stream 
            virtual size_t printTo(Print& p) const = 0;

            // Generates the compact JSON string and writes it in a buffer
            size_t printTo(char* buffer, size_t bufferSize) const;

            // Generates the indented JSON string and sends it to a Print stream
            size_t prettyPrintTo(Print& p) const;

            // Generates the indented JSON string and sends it to a IndentedPrint stream
            // This overload allows a finer control of the output because you can customize
            // the IndentedPrint.
            size_t prettyPrintTo(IndentedPrint& p) const;

            // Generates the indented JSON string and writes it in a buffer
            size_t prettyPrintTo(char* buffer, size_t bufferSize) const;
        };
    }
}
