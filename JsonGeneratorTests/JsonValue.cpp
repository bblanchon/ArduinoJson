/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"
#include "JsonObjectBase.h"
#include <cstdio>
#include <cstring>

void JsonValue::writeBooleanTo(JsonSink& sb)
{
    sb.append(content.boolean ? "true" : "false");
}

void JsonValue::writeNumberTo(JsonSink& sb)
{
    char tmp[16];

    _snprintf(tmp, sizeof(tmp), "%lg", content.number);

    sb.append(tmp);
}

void JsonValue::writeObjectTo(JsonSink& sink)
{
    if (content.object)
        ((JsonObjectBase*) content.object)->writeTo(sink);
    else
        sink.append("null");
}

void JsonValue::writeStringTo(JsonSink& sink)
{
    auto s = content.string;

    if (!s)
    {
        return sink.append("null");
    }

    if (!sink.hasRoomFor(2))
    {
        return;
    }

    sink.append('\"');
    sink.reserveRoom(1);

    while (*s)
    {
        switch (*s)
        {
        case '"':
            sink.append("\\\"");
            break;

        case '\\':
            sink.append("\\\\");
            break;

        case '\b':
            sink.append("\\b");
            break;

        case '\f':
            sink.append("\\f");
            break;

        case '\n':
            sink.append("\\n");
            break;

        case '\r':
            sink.append("\\r");
            break;

        case '\t':
            sink.append("\\t");
            break;

        default:
            sink.append(*s);
            break;
        }

        s++;
    }

    sink.releaseRoom(1);
    sink.append('\"');
}