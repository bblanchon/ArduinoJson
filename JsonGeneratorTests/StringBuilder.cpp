/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "StringBuilder.h"

void StringBuilder::append(const char* s)
{
    char* tail = buffer + length;

    while (*s && length<capacity)
    {
        buffer[length++] = *s++;
    }

    buffer[length] = 0;
}


void StringBuilder::append(char c)
{
    if (length >= capacity) return;

    buffer[length++] = c;
    buffer[length] = 0;
}