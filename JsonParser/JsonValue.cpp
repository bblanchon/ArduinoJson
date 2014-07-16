/*
 * Arduino JSON library
 * Benoit Blanchon 2014 - MIT License
 */

#include <stdlib.h> // for strtol, strtod
#include "JsonArray.h"
#include "JsonHashTable.h"
#include "JsonValue.h"

using namespace ArduinoJson::Parser;

JsonValue::operator bool()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE) return 0;

    // "true"
    if (json[tokens->start] == 't') return true;

    // "false"
    if (json[tokens->start] == 'f') return false;

    // "null"
    if (json[tokens->start] == 'n') return false;

    // number
    return strtol(json + tokens->start, 0, 0) != 0;
}

JsonValue::operator double()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE) return 0;

    return strtod(json + tokens->start, 0);
}

JsonValue::operator long()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE) return 0;

    return strtol(json + tokens->start, 0, 0);
}

JsonValue::operator char*()
{
    if (tokens == 0 || tokens->type != JSMN_PRIMITIVE && tokens->type != JSMN_STRING)
        return 0;

    // add null terminator to the string
    json[tokens->end] = 0;

    return json + tokens->start;
}

JsonValue::operator JsonArray()
{
    return JsonArray(json, tokens);
}

JsonValue::operator JsonHashTable()
{
    return JsonHashTable(json, tokens);
}