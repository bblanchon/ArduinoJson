/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "StringBuilder.h"

using namespace ArduinoJson::Internals;

size_t StringBuilder::write(uint8_t c)
{
    if (length >= capacity) return 0;

    buffer[length++] = c;
    buffer[length] = 0;
    return 1;
}