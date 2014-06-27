/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include "JsonValue.h"
#include "JsonObjectBase.h"

void JsonValue::writeTo(StringBuilder& sb)
{
    switch (type)
    {
    case JSON_STRING:
        sb.appendEscaped(content.string);
        break;

    case JSON_NUMBER:
        sb.append(content.number);
        break;

    case JSON_BOOLEAN:
        sb.append(content.boolean ? "true" : "false");
        break;

    case JSON_OBJECT:
        if (content.object)
            ((JsonObjectBase*)content.object)->writeTo(sb);
        else
            sb.append("null");
        break;
    }
}