/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"
#include "JsonObjectBase.h"
#include <cstdio>
#include <cstring>

size_t JsonValue::writeBooleanTo(JsonSink& sb)
{
    return sb.write(content.boolean ? "true" : "false");
}

size_t JsonValue::writeNumberTo(JsonSink& sb)
{
    char tmp[16];

    _snprintf(tmp, sizeof(tmp), "%lg", content.number);

    return sb.write(tmp);
}

size_t JsonValue::writeObjectTo(JsonSink& sink)
{
    if (content.object)
        return ((JsonObjectBase*)content.object)->writeTo(sink);
    else
        return sink.write("null");
}

size_t JsonValue::writeStringTo(JsonSink& sink)
{
    auto s = content.string;

    if (!s)
    {
        return sink.write("null");
    }

    size_t n = 0;

    n += sink.write('\"');

    while (*s)
    {
        switch (*s)
        {
        case '"':
            n += sink.write("\\\"");
            break;

        case '\\':
            n += sink.write("\\\\");
            break;

        case '\b':
            n += sink.write("\\b");
            break;

        case '\f':
            n += sink.write("\\f");
            break;

        case '\n':
            n += sink.write("\\n");
            break;

        case '\r':
            n += sink.write("\\r");
            break;

        case '\t':
            n += sink.write("\\t");
            break;

        default:
            n += sink.write(*s);
            break;
        }

        s++;
    }

    n += sink.write('\"');

    return n;
}