/*
* Arduino JSON library
* Benoit Blanchon 2014 - MIT License
*/

#include "JsonPrintable.h"
#include "JsonPrettyPrint.h"
#include "StringBuilder.h"

using namespace ArduinoJson::Generator;
using namespace ArduinoJson::Internals;

size_t JsonPrintable::printTo(char* buffer, size_t bufferSize) const
{
    StringBuilder sb(buffer, bufferSize);
    return printTo(sb);
}

size_t JsonPrintable::prettyPrintTo(char* buffer, size_t bufferSize) const
{
    StringBuilder sb(buffer, bufferSize);
    return prettyPrintTo(sb);
}

size_t JsonPrintable::prettyPrintTo(IndentedPrint& p) const
{
    JsonPrettyPrint prettyPrint(p);
    return printTo(prettyPrint);
}

size_t JsonPrintable::prettyPrintTo(Print& p) const
{
    IndentedPrint indentedPrint(p);
    return prettyPrintTo(indentedPrint);
}
