/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <stdlib.h> // for strtol, strtod
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

JsonValue JsonValue::operator[](int index)
{
    return JsonArray(*this)[index];
}

JsonValue JsonValue::operator[](const char* key)
{
    return JsonObject(*this)[key];
}

JsonValue::operator bool()
{
    if (!isPrimitive()) return 0;

    char *text = getText();

    // "true"
    if (text[0] == 't') return true;

    // "false"
    if (text[0] == 'f') return false;

    // "null"
    if (text[0] == 'n') return false;

    // number
    return strtol(text, 0, 0) != 0;
}

JsonValue::operator double()
{
    return isPrimitive() ? strtod(getText(), 0) : 0;
}

JsonValue::operator long()
{
    return isPrimitive() ? strtol(getText(), 0, 0) : 0;
}

JsonValue::operator char*()
{
    return isString() || isPrimitive() ? getText() : 0;
}