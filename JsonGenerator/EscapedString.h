/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#pragma once

#include "Print.h"

namespace ArduinoJson
{
    namespace Internals
    {
        class EscapedString
        {
        public:

            EscapedString(const char* s)
                : rawString(s)
            {                
            }

            size_t printTo(Print&) const;
            
        private:
            const char* rawString;
        };
    }
}