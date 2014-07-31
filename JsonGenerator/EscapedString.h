/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "Print.h"
#include <string.h> // for strcmp

namespace ArduinoJson
{
    namespace Internals
    {
        class EscapedString
        {
        public:

            void set(const char* s)
            {
                rawString = s;
            }

            size_t printTo(Print&) const;
            
            bool equals(char const* s)
            {
                return strcmp(s, rawString) == 0;
            }

        private:
            const char* rawString;
        };
    }
}