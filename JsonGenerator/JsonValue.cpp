/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"

using namespace ArduinoJson::Generator;

size_t JsonValue::printBoolTo(const Content& c, Print& p)
{
    return p.print(c.asBool ? "true" : "false");
}

size_t JsonValue::printLongTo(const Content& c, Print& p)
{
    return p.print(c.asLong);
}

size_t JsonValue::printPrintableTo(const Content& c, Print& p)
{
    if (c.asPrintable)
        return ((Printable*) c.asPrintable)->printTo(p);
    else
        return p.print("null");
}

size_t JsonValue::printStringTo(const Content& c, Print& p)
{
    const char* s = c.asString;

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