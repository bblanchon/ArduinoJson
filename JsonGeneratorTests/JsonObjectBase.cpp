#include "JsonObjectBase.h"

void JsonObjectBase::writeValueTo(JsonValue& obj, StringBuilder& sb)
{
    switch (obj.type)
    {
    case JSON_STRING:
        sb.appendEscaped(obj.content.string);
        break;

    case JSON_NUMBER:
        sb.append(obj.content.number);
        break;

    case JSON_BOOLEAN:
        sb.append(obj.content.boolean ? "true" : "false");
        break;

    case JSON_OBJECT:
        if (obj.content.object)
            obj.content.object->writeTo(sb);
        else
            sb.append("null");
        break;
    }
}