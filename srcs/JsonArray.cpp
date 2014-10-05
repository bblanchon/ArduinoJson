#include "JsonArray.h"

#include "JsonValue.h"

JsonValue JsonArray::operator[](int index) const
{
    for (JsonNodeIterator it = beginChildren(); it != endChildren(); ++it)
    {
        if (!index) return JsonValue(*it);
        index--;
    }

    return JsonValue();
}