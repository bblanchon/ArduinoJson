/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"
#include "JsonObjectBase.h"
#include <cstdio>
#include <cstring>

size_t JsonValue::writeBooleanTo(Print& p) const
{
    return p.write(content.boolean ? "true" : "false");
}

size_t JsonValue::writeNumberTo(Print& p) const
{
    char tmp[16];

    _snprintf(tmp, sizeof(tmp), "%lg", content.number);

    return p.write(tmp);
}

size_t JsonValue::writeObjectTo(Print& p) const
{
    if (content.object)
        return ((JsonObjectBase*)content.object)->writeTo(p);
    else
        return p.write("null");
}

size_t JsonValue::writeStringTo(Print& p) const
{
    auto s = content.string;

    if (!s)
    {
        return p.write("null");
    }

    size_t n = 0;

    n += p.write('\"');

    while (*s)
    {
        switch (*s)
        {
        case '"':
            n += p.write("\\\"");
            break;

        case '\\':
            n += p.write("\\\\");
            break;

        case '\b':
            n += p.write("\\b");
            break;

        case '\f':
            n += p.write("\\f");
            break;

        case '\n':
            n += p.write("\\n");
            break;

        case '\r':
            n += p.write("\\r");
            break;

        case '\t':
            n += p.write("\\t");
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