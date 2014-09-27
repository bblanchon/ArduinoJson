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
            static size_t printTo(const char*, Print&);
        };
    }
}
