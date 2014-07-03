/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#ifndef ARDUINO

#include "Print.h"
#include <cstdio>
//#include <cstring>

size_t Print::write(const char* s)
{
    size_t n = 0;
    while (*s)
    {
        n += write(*s++);
    }
    return n;
}

size_t Print::print(double value, int digits)
{
    char tmp[32];
    sprintf(tmp, "%.*lg", digits+1, value);
    return write(tmp);
}

size_t Print::print(long value)
{
    char tmp[32];
    sprintf(tmp, "%ld", value);
    return write(tmp);
}

#endif