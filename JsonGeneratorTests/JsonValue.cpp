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
    return sb.append(content.boolean ? "true" : "false");
}

size_t JsonValue::writeNumberTo(JsonSink& sb)
{
    char tmp[16];

    _snprintf(tmp, sizeof(tmp), "%lg", content.number);

    return sb.append(tmp);
}

size_t JsonValue::writeObjectTo(JsonSink& sink)
{
    if (content.object)
        return ((JsonObjectBase*)content.object)->writeTo(sink);
    else
        return sink.append("null");
}

size_t JsonValue::writeStringTo(JsonSink& sink)
{
    auto s = content.string;

    if (!s)
    {
        return sink.append("null");
    }

    size_t n = 0;

    n += sink.append('\"');

    while (*s)
    {
        switch (*s)
        {
        case '"':
            n += sink.append("\\\"");
            break;

        case '\\':
            n += sink.append("\\\\");
            break;

        case '\b':
            n += sink.append("\\b");
            break;

        case '\f':
            n += sink.append("\\f");
            break;

        case '\n':
            n += sink.append("\\n");
            break;

        case '\r':
            n += sink.append("\\r");
            break;

        case '\t':
            n += sink.append("\\t");
            break;

        default:
            n += sink.append(*s);
            break;
        }

        s++;
    }

    n += sink.append('\"');

    return n;
}