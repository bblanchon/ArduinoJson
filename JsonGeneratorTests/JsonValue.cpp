/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"
#include "JsonObjectBase.h"

void JsonValue::writeBooleanTo(StringBuilder& sb)
{
    sb.append(content.boolean ? "true" : "false");
}

void JsonValue::writeNumberTo(StringBuilder& sb)
{
    sb.append(content.number);
}

void JsonValue::writeObjectTo(StringBuilder& sb)
{
    if (content.object)
        ((JsonObjectBase*) content.object)->writeTo(sb);
    else
        sb.append("null");
}

void JsonValue::writeStringTo(StringBuilder& sb)
{
    sb.appendEscaped(content.string);
}