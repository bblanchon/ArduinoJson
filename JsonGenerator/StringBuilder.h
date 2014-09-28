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
        class StringBuilder : public Print
        {
        public:
            StringBuilder(char* buf, int size)
                : _buffer(buf), _capacity(size - 1), _length(0)
            {
                _buffer[0] = 0;
            }

            virtual size_t write(uint8_t c);

        private:
            char* _buffer;
            int _capacity;
            int _length;
        };
    }
}
