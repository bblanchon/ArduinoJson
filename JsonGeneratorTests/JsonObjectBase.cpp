#include "JsonObjectBase.h"

void JsonObjectBase::writeObjectTo(ObjectContainer& obj, StringBuilder& sb)
{
    switch (obj.type)
    {
    case JSON_STRING:
        if (obj.value.string)
            sb.append("\"%s\"", obj.value.string);
        else
            sb.append("null");
        break;

    case JSON_NUMBER:
        sb.append("%lg", obj.value.number);
        break;

    case JSON_BOOLEAN:
        sb.append(obj.value.boolean ? "true" : "false");
        break;

    case JSON_OBJECT:
        if (obj.value.object)
            obj.value.object->writeTo(sb);
        else
            sb.append("null");
        break;
    }
}