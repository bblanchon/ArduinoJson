/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"

using namespace ArduinoJson::Generator;

size_t JsonValue::printBoolTo(Print& p) const
{
    return p.print(content.asBool ? "true" : "false");
}

size_t JsonValue::printDoubleTo(Print& p) const
{
    return p.print(content.asDouble.value, content.asDouble.digits);
}

size_t JsonValue::printFloatTo(Print& p) const
{
    return p.print(content.asFloat);
}

size_t JsonValue::printLongTo(Print& p) const
{
    return p.print(content.asLong);
}

size_t JsonValue::printPrintableTo(Print& p) const
{
    if (content.asPrintable)
        return ((Printable*) content.asPrintable)->printTo(p);
    else
        return p.print("null");
}

size_t JsonValue::printStringTo(Print& p) const
{
    const char* s = content.asString;

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