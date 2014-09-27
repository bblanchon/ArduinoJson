/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#ifndef ARDUINO

#include "Print.h"
#include <string> // for sprintf, strchr and strcat

size_t Print::print(const char s[])
{
    size_t n = 0;
    while (*s)
    {
        n += write(*s++);
    }
    return n;
}

static inline void ensureStringContainsPoint(char* s)
{
    // Ensures that the decimal point is present.
    // For example, we don't want "0" but "0.0".
    // Otherwise, the value would be considered as an integer by some parsers
    // See issue #22

    if (!strchr(s, '.'))
        strcat(s, ".0");
}

size_t Print::print(double value, int digits)
{
    char tmp[32];

    sprintf(tmp, "%.*lg", digits+1, value);

    if (digits>0)
        ensureStringContainsPoint(tmp);

    return print(tmp);
}

size_t Print::print(long value)
{
    char tmp[32];
    sprintf(tmp, "%ld", value);
    return print(tmp);
}

size_t Print::println()
{
    return write('\r') + write('\n');
}

#endif
