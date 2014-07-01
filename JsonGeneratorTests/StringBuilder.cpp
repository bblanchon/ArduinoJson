/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "StringBuilder.h"

size_t StringBuilder::write(char c)
{
    if (length >= capacity) return 0;

    buffer[length++] = c;
    buffer[length] = 0;
    return 1;
}