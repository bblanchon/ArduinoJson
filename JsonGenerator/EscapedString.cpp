/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "EscapedString.h"


size_t EscapedString::printTo(Print& p) const
{
    const char* s = rawString;

    if (!s)
    {
        return p.print("null");
    }

    size_t n = 0;

    n += p.write('\"');

    while (*s)
    {
        switch (*s)
        {
        case '"':
            n += p.print("\\\"");
            break;

        case '\\':
            n += p.print("\\\\");
            break;

        case '\b':
            n += p.print("\\b");
            break;

        case '\f':
            n += p.print("\\f");
            break;

        case '\n':
            n += p.print("\\n");
            break;

        case '\r':
            n += p.print("\\r");
            break;

        case '\t':
            n += p.print("\\t");
            break;

        default:
            n += p.write(*s);
            break;
        }

        s++;
    }

    n += p.write('\"');

    return n;
}