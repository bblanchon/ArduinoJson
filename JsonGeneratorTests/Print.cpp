/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "Print.h"

size_t Print::write(const char* s)
{
    size_t n = 0;
    while (*s)
    {
        n += write(*s++);
    }
    return n;
}