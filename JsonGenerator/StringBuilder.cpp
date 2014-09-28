/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "StringBuilder.h"

using namespace ArduinoJson::Internals;

size_t StringBuilder::write(uint8_t c)
{
    if (_length >= _capacity) return 0;

    _buffer[_length++] = c;
    _buffer[_length] = 0;
    return 1;
}
