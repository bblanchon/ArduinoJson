/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "StringBuilder.h"


void StringBuilder::append(const char* s)
{
    if (!s)
    {
        return append("null");        
    }

    char* tail = buffer + length;

    while (*s && length<capacity)
    {
        buffer[length++] = *s++;
    }

    buffer[length] = 0;
}

void StringBuilder::appendEscaped(const char* s)
{
    if (!s)
    {
        return append("null");
    }

    if (length > capacity - 2)
    {
        // not enough from for quotes
        return;
    }

    buffer[length++] = '"';

    // keep one slot for the end quote
    capacity--;

    while (*s && length < capacity)
    {
        switch (*s)
        {
        case '"':
            append("\\\"");
            break;

        case '\\':
            append("\\\\");
            break;

        case '\b':
            append("\\b");
            break;

        case '\f':
            append("\\f");
            break;

        case '\n':
            append("\\n");
            break;

        case '\r':
            append("\\r");
            break;

        case '\t':
            append("\\t");
            break;

        default:
            buffer[length++] = *s;
            break;
        }

        s++;
    }

    buffer[length++] = '"';
    buffer[length] = 0;

    // restore the original capacity
    capacity++;
}

void StringBuilder::appendFormatted(const char* format, ...)
{
    char* tail = buffer + length;

    va_list args;
    va_start(args, format);
    vsnprintf(tail, capacity - length, format, args);
    va_end(args);

    length += strlen(tail);
}