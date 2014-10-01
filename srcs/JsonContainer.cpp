#include "JsonContainer.h"

#include "Internals/JsonNodeSerializer.h"
#include "Internals/StringBuilder.h"

size_t JsonContainer::printTo(char* buffer, size_t bufferSize) const
{
    ArduinoJson::Internals::StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
}

size_t JsonContainer::printTo(Print& p) const
{
    JsonNodeSerializer serializer(p);
    return serializer.serialize(_node);
}